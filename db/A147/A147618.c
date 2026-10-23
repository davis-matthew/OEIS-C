/*
 * A147618  The 3rd Witt transform of A000217.
 * https://oeis.org/A147618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 67, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A147618
 *
 * Definition:  G(x) = 3*x^4*(1-x+3*x^2-x^3+x^4) /
 *                 ((1-x)^9 * (1+x+x^2)^3)
 * The coefficients of the denominator are
 *     d = 1, -6,  15, -23,  33, -51,  64, -63,  63,
 *         -64, 51, -33,  23, -15,   6, -1      (length 16)
 *
 * Let b_n be the coefficient of x^n in the numerator:
 *     b_4 = 3,  b_5 = -3,  b_6 = 9,  b_7 = -3,  b_8 = 3,
 *     b_n = 0 otherwise.
 *
 * Then a(n) satisfies the linear recurrence
 *
 *     a(n) = b_n - Σ_{k=1}^{15} d_k · a(n-k)    (n ≥ 0),
 *
 * with a(m) = 0 for m < 0.
 *
 * The implementation computes a(n) on demand, caching already
 * computed values.  All arithmetic is performed with the seq_t type
 * so that it works for unsigned integer builds and for the big‐int
 * build.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cached values of a(n)   */
static idx_t cache_len = 0;      /* number of cached values */

/* denominator coefficients d_k, k = 0 .. 15 (d_0 == 1) */
static const int d[16] = {
    1,  -6,  15, -23,  33, -51,  64, -63,  63,
   -64,  51, -33,  23, -15,   6, -1
};

/* numerator coefficients b_n, n = 0 .. 8  (0 for other indices) */
static const int b[9] = {
    0,  /* n=0 */
    0,  /* n=1 */
    0,  /* n=2 */
    0,  /* n=3 */
    3,  /* n=4 */
   -3,  /* n=5 */
    9,  /* n=6 */
   -3,  /* n=7 */
    3   /* n=8 */
};

seq_t a(idx_t n)
{
    /* Expand cache if necessary */
    if (n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        cache = new_cache;
        cache_len = new_len;

        /* Compute new terms */
        for (idx_t i = old_len; i < new_len; ++i) {
            seq_t acc = 0;                /* accumulate as seq_t (signed if needed) */

            /* add b_i */
            if (i <= 8) acc += (seq_t)b[i];  /* b[i] fits in int */

            /* subtract d_k * a(i-k) for k = 1 .. 15 */
            for (int k = 1; k <= 15; ++k) {
                if (i < (idx_t)k) continue;   /* a(i-k) == 0 for negative index */

                seq_t term = cache[i - k];
                if (d[k] > 0) {
                    /* d_k > 0  ->  acc -= d_k * term */
                    acc -= (seq_t)(d[k]) * term;
                } else if (d[k] < 0) {
                    /* d_k < 0  ->  acc += (-d_k) * term */
                    acc += (seq_t)(-d[k]) * term;
                }
                /* d_k == 0 does nothing */
            }

            cache[i] = acc;     /* final value */
        }
    }
    return cache[n];
}
