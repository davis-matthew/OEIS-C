/*
 * A164787  Polynomial expansion of p(x)=1/(1 - 3 x + 2 x^2 + 2 x^3 - 4 x^4 +
 *          4 x^5 - 2 x^6 - 2 x^7 + 3 x^8 - x^9 - x^17 + 3 x^18 - 2 x^19 - 2
 *          x^20 + 4 x^21 - 4 x^22 + 2 x^23 + 2 x^24 - 3 x^25 + x^26).
 * https://oeis.org/A164787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = coefficient of x^n in 1/(1 - 3x + 2x^2 + 2x^3 - 4x^4 +
 *            4x^5 - 2x^6 - 2x^7 + 3x^8 - x^9 - x^17 + 3x^18 - 2x^19 - 2x^20 +
 *            4x^21 - 4x^22 + 2x^23 + 2x^24 - 3x^25 + x^26)
 * Range:     u8 n <= 10, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence satisfies a linear recurrence of order 26 derived from the denominator polynomial. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Declare static cache for memoisation. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* allocated bytes/slots */
    static size_t cache_used = 0;   /* number of values computed (cache[0..cache_used-1]) */

    /* Coefficients of the denominator (index 0 unused). */
    static const int coeff[27] = {
        0,   /* d0=1, unused for recurrence */
        -3,  2,   2,  -4,  4,  -2,  -2,   3,  -1,   0,   0,   0,   0,   0,   0,   0,
        -1,  3,  -2,  -2,   4, -4,   2,   2,  -3,   1
    };  /* indices 1..26 correspond to x..x^26 */

    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        cache_len = 128; /* start with some capacity */
        cache     = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;            /* a(0) = 1 */
        cache_used = 1;          /* one value computed */
    }

    /* Ensure the cache is large enough for n. */
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        if (new_len < cache_len * 2) new_len = cache_len * 2; /* growth factor 2 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (new_cache) {
            cache = new_cache;
            cache_len = new_len;
        }
    }

    /* Compute any missing values up to n. */
    if (n >= cache_used) {
        for (size_t i = cache_used; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            size_t limit = (i < 26) ? i : 26;
            for (size_t k = 1; k <= limit; ++k) {
                int d = coeff[(size_t)k];
                if (d > 0) {
                    /* subtract positive contribution: sum -= d * a(i-k) */
                    sum -= (seq_t)d * cache[i - k];
                } else if (d < 0) {
                    /* add positive contribution: sum += (-d) * a(i-k) */
                    sum += (seq_t)(-d) * cache[i - k];
                }
            }
            cache[i] = sum;
        }
        cache_used = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
