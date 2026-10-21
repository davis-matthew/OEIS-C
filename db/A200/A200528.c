/*
 * A200528  Number of n X 2 0..2 arrays with every row and column running
 *          average nondecreasing rightwards and downwards.
 * https://oeis.org/A200528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 16*a(n-2) + 6*a(n-3) + 27*a(n-4) - 33*a(n-5) -
 *            3*a(n-6) + 24*a(n-7) - 11*a(n-8) - 4*a(n-9) + 4*a(n-10)
 *            (empirical recurrence, a(1)=6, a(2)=20, a(3)=57, a(4)=146,
 *            a(5)=354, a(6)=825, a(7)=1873, a(8)=4169, a(9)=9144,
 *            a(10)=19825)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {
    0ULL,           /* index 0 unused */
    6ULL, 20ULL, 57ULL, 146ULL, 354ULL,
    825ULL, 1873ULL, 4169ULL, 9144ULL, 19825ULL
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoization table, 1‑based */
    static idx_t  cache_len = 0;         /* greatest index computed */

    /* Initialise cache with the known first 10 values */
    if (cache == NULL) {
        cache_len = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
        if (!cache) return (seq_t)0;   /* allocation failure – return 0 */
        for (idx_t i = 1; i <= cache_len; ++i)
            cache[i] = init_vals[i];
        cache[0] = (seq_t)0;           /* unused but keeps indices clean */
    }

    /* Grow cache if necessary */
    if (n > cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!cache) return (seq_t)0;  /* allocation failure – return 0 */

        /* Compute new terms using the recurrence */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Sum of terms with positive coefficients */
            seq_t pos = (seq_t)0;
            pos += cache[i - 1] * (seq_t)7;
            pos += cache[i - 3] * (seq_t)6;
            pos += cache[i - 4] * (seq_t)27;
            pos += cache[i - 7] * (seq_t)24;
            pos += cache[i -10] * (seq_t)4;

            /* Sum of terms with negative coefficients */
            seq_t neg = (seq_t)0;
            neg += cache[i - 2] * (seq_t)16;
            neg += cache[i - 5] * (seq_t)33;
            neg += cache[i - 6] * (seq_t)3;
            neg += cache[i - 8] * (seq_t)11;
            neg += cache[i - 9] * (seq_t)4;

            cache[i] = pos - neg;          /* pos >= neg by construction */
        }
        cache_len = n;
    }

    return cache[n];
}
