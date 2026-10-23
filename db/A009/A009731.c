/*
 * A009731  Expansion of tan(x)*cosh(log(1+x)).
 * https://oeis.org/A009731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * [coeff. of x^n in tan(x)*cosh(log(1+x))]
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from: f(x) = tan(x)*cosh(log(1+x)) =
   f(x) + x*f(x) = tan(x)*(1+x+x^2/2)  => f[n] = g[n]-f[n-1]   */
/* where g[n] = tan[n] + tan[n-1] + tan[n-2]/2 and a[n] = n!*f[n]  */
/* b[n] = n!*tan[n] satisfies: b[0]=0, b[1]=1,
   b[n] = Σ_{k=0}^{n-1} C(n-1,k) * b[k] * b[n-1-k]  (n≥2)   */
/* a recurrence: a[0]=0; for n≥1:
      a[n] = b[n] + n*b[n-1] + (n*(n-1)/2)*b[n-2] - n*a[n-1] */

/* All arithmetic is performed in seq_t, which may be int64_t, __int128,
   or an arbitrary-precision integer type provided by the build. */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static idx_t a_size = 0;
static idx_t b_size = 0;

/* Ensure that cache arrays are allocated up to at least n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (b_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_b = (seq_t *)realloc(b_cache, new_size * sizeof(seq_t));
        if (!new_b) exit(1);
        b_cache = new_b;
        /* initialize new entries */
        for (idx_t i = b_size; i < new_size; ++i)
            b_cache[i] = 0;
        /* compute new tangent numbers */
        for (idx_t i = b_size; i <= n; ++i) {
            if (i == 0) { b_cache[0] = 0; continue; }
            if (i == 1) { b_cache[1] = 1; continue; }
            seq_t sum = 0;
            seq_t comb = 1;                  /* C(i-1,0) */
            for (idx_t k = 0; k < i; ++k) {
                if (b_cache[k] && b_cache[i-1-k]) {
                    sum += comb * (b_cache[k] * b_cache[i-1-k]);
                }
                if (k < i - 1) {                    /* update C(i-1,k+1) */
                    comb = comb * (seq_t)(i-1 - k) / (k + 1);
                }
            }
            b_cache[i] = sum;
        }
        b_size = new_size;
    }
    if (a_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_a = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
        if (!new_a) exit(1);
        a_cache = new_a;
        /* initialize new entries */
        for (idx_t i = a_size; i < new_size; ++i)
            a_cache[i] = 0;
        /* compute new a values, relying on already computed b */
        for (idx_t i = a_size; i <= n; ++i) {
            if (i == 0) { a_cache[0] = 0; continue; }
            seq_t res = b_cache[i];
            res += (seq_t)i * b_cache[i-1];
            if (i >= 2) {
                seq_t mult = ((seq_t)i * (seq_t)(i-1)) / 2;
                res += mult * b_cache[i-2];
            }
            res -= (seq_t)i * a_cache[i-1];
            a_cache[i] = res;
        }
        a_size = new_size;
    }
}

/* Main function requested by the judge. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
