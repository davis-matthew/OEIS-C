/*
 * A165189  Partial sums of partial sums of (A001840 interleaved with zeros).
 * https://oeis.org/A165189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies a linear recurrence of order 12, derived from the
 *            generating function a(x) = x /
 *            ((1-x)^5*(1+x)^3*(1-x+x^2)*(1+x+x^2)) The coefficients of
 *            (1-x)^5*(1+x)^3*(1-x+x^2)*(1+x+x^2) are: 1, -2, -1, 4, -1, -2,
 *            0, 2, 1, -4, 1, 2, -1 Hence for n ≥ 13 a(n) = 2*a(n-1) + a(n-2)
 *            - 4*a(n-3) + a(n...
 * Range:     u8 n <= 14, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t base_vals[12] = {
    1, 2, 5, 8, 14, 20, 31, 42, 60, 78, 105, 132
};

static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of terms computed (a(1)..a(cache_len)) */

/* Compute a(n), n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, return 0 */

    /* initialise cache with the first 12 terms once */
    if (cache == NULL) {
        cache_len = 12;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = base_vals[i];
    }

    /* extend cache up to n if necessary */
    if ((idx_t)cache_len < n) {
        size_t new_len = (size_t)n;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* compute missing terms using the recurrence */
        for (size_t idx = cache_len; idx < new_len; ++idx) {
            /* idx corresponds to a(idx+1) */
            seq_t sumPos = 2 * cache[idx - 1]      /* 2*a(n-1)  */
                        + cache[idx - 2]            /*   + a(n-2) */
                        + cache[idx - 4]            /*   + a(n-4) */
                        + 2 * cache[idx - 5]        /*   + 2*a(n-5) */
                        + 4 * cache[idx - 9]        /*   + 4*a(n-9) */
                        + cache[idx - 12];          /*   + a(n-12) */

            seq_t sumNeg = 4 * cache[idx - 3]      /* -4*a(n-3)  */
                        + 2 * cache[idx - 7]        /*  -2*a(n-7) */
                        + cache[idx - 8]            /*  -a(n-8)   */
                        + cache[idx - 10]           /*  -a(n-10)  */
                        + 2 * cache[idx - 11];      /*  -2*a(n-11) */

            cache[idx] = sumPos - sumNeg;
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
