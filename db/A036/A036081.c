/*
 * A036081  The number of partitions of {1..(11n)} that are invariant under a
 *          permutation consisting of n 11-cycles.
 * https://oeis.org/A036081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   The sequence is defined by the exponential generating function
 *            f(x) = exp((exp(11x)-11-1)/11 + exp(x)). A direct recurrence for
 *            its coefficients a(n) = f_n is: a(0) = 1 a(n+1) = Σ_{k=0}^{n}
 *            C(n,k) * (11^{n-k} + 1) * a(k) where C(n,k) are binomial
 *            coefficients. The implementation below memoises te...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *f = NULL;          /* memoised values a(0..max_n)          */
static seq_t *pow11 = NULL;      /* powers of 11 up to used index       */
static size_t capacity = 0;      /* current allocation size             */
static idx_t cached_max = 0;     /* largest n for which a(n) is cached */

/* Ensure allocation big enough for indices 0..n (inclusive).  */
static void ensure_capacity(idx_t n)
{
    if (capacity <= (size_t)n) {
        size_t oldcap = capacity;
        /* Grow roughly to double the size to keep reallocs cheap.   */
        size_t newcap = (capacity == 0) ? 1 : (capacity << 1);
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;

        seq_t *new_f   = (seq_t *)malloc(newcap * sizeof(seq_t));
        seq_t *new_pow = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!new_f || !new_pow) { /* allocation failed – abort softly */
            return;
        }

        /* copy old data if any */
        if (f)   memcpy(new_f,   f,   oldcap * sizeof(seq_t));
        if (pow11) memcpy(new_pow, pow11, oldcap * sizeof(seq_t));

        /* free old buffers */
        free(f);   free(pow11);

        f = new_f;   pow11 = new_pow;
        capacity = newcap;

        /* initialise newly allocated slots */
        if (oldcap == 0) {
            /* first element f[0] = 1, pow11[0] = 1 */
            f[0] = 1;
            pow11[0] = 1;
        }
        /* compute missing powers of 11 */
        for (size_t i = oldcap; i < capacity; ++i) {
            if (i == 0) continue;
            pow11[i] = pow11[i-1] * (seq_t)11;
        }
    }
}

/* Compute terms a(k) for k = cached_max+1 .. n using the recurrence. */
static void compute_to(idx_t n)
{
    for (idx_t m = cached_max + 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t bin = 1;                      /* C(m-1,0) = 1      */
        for (idx_t k = 0; k <= m - 1; ++k) {
            idx_t exp_idx = m - 1 - k;        /* exponent for 11   */
            seq_t a_val = pow11[exp_idx] + (seq_t)1;   /* 11^{exp_idx} + 1 */
            seq_t temp = f[k] * a_val;         /* f(k) * a_val      */
            temp = temp * bin;                 /* multiply by binom  */
            sum = sum + temp;                  /* accumulate          */

            if (k < m - 1) {
                /* update binomial coefficient: C(n,k+1) = C(n,k) * (n-k)/(k+1) */
                bin = bin * (seq_t)(m - 1 - k);
                bin = bin / (seq_t)(k + 1);
            }
        }
        f[m] = sum;
    }
    cached_max = n;
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n > cached_max) {
        compute_to(n);
    }
    return f[n];
}
