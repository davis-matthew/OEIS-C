/*
 * A193377  Real part of g.f. C(x) that satisfies: C(x) = 1 + x*C(I*x)^2.
 * https://oeis.org/A193377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Real part of the coefficient of x^n in C(x), where C(x) =
 *            1 + x * C(i*x)^2. The recurrence for the complex coefficients
 *            c_n = r_n + i*s_n is c_{n+1} = i^n * sum_{k=0}^n c_k c_{n-k},
 *            which yields the real/imag parts update below
 * Range:     i8 n <= 8, i16 n <= 14, i32 n <= 27, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* memoized arrays for real (r) and imaginary (s) parts of c_n */
static seq_t *r = NULL;      /* r[n] = Re(c_n) */
static seq_t *s = NULL;      /* s[n] = Im(c_n) */
static size_t capacity = 0;  /* allocated length  (>= last+1) */
static idx_t last   = 0;     /* highest index computed */
static int   ready  = 0;     /* 0 = not initialized yet */

/* initialise the first coefficient c_0 = 1 + 0*i */
static void init(void)
{
    capacity = 1;
    r = (seq_t *)malloc(capacity * sizeof(seq_t));
    s = (seq_t *)malloc(capacity * sizeof(seq_t));
    r[0] = 1;   s[0] = 0;
    last = 0;
    ready = 1;
}

/* extend the memoised arrays to include coefficients up to index n */
static void grow_to(idx_t n)
{
    if (!ready) init();
    if (n <= last) return;          /* already computed */

    /* grow storage if needed */
    if ((size_t)n >= capacity) {
        size_t new_cap = capacity;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        r = (seq_t *)realloc(r, new_cap * sizeof(seq_t));
        s = (seq_t *)realloc(s, new_cap * sizeof(seq_t));
        /* initialise new slots to zero */
        for (size_t i = capacity; i < new_cap; ++i) {
            r[i] = 0;
            s[i] = 0;
        }
        capacity = new_cap;
    }

    /* compute missing coefficients */
    for (idx_t i = last + 1; i <= n; ++i) {
        idx_t nn = i - 1;           /* n in the recurrence   */
        seq_t R = 0;                /* real part of sum A_n   */
        seq_t S = 0;                /* imag part of sum A_n   */
        for (idx_t k = 0; k <= nn; ++k) {
            seq_t rk   = r[k];
            seq_t rnk  = r[nn - k];
            seq_t sk   = s[k];
            seq_t snk  = s[nn - k];
            seq_t termR = rk * rnk - sk * snk;
            seq_t termS = rk * snk + sk * rnk;
            R += termR;
            S += termS;
        }
        switch (nn & 3) {
            case 0: r[i] = R;          s[i] = S;          break;
            case 1: r[i] = -S;         s[i] = R;          break;
            case 2: r[i] = -R;         s[i] = -S;         break;
            default: r[i] = S;         s[i] = -R;         break;
        }
    }
    last = n;
}

/* public function required by the framework */
seq_t a(idx_t n)
{
    grow_to(n);
    return r[n];
}
