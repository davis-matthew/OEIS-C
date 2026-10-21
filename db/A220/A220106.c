/*
 * A220106  Number of nX1 arrays of the minimum or maximum value of
 *          corresponding elements and their horizontal and vertical neighbors
 *          in a random 0..3 nX1 array.
 * https://oeis.org/A220106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) + 3*a(n-3) -32*a(n-4) -34*a(n-5)
 *            -23*a(n-6) -158*a(n-7) +295*a(n-9) +42*a(n-10) -65*a(n-11)
 *            +12*a(n-12) -44*a(n-13) +2*a(n-14) +24*a(n-15) Initial values
 *            (n=1..15): 4, 16, 56, 250, 984, 3888, 15798, 63092, 252064,
 *            1010554, 4040556, 16155482, 64620946, 258358476, 1032891644
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity */
    static size_t cache_len = 0;      /* largest n computed (<= cache_len) */

    if (n == 0)                 /* sequence is undefined for n<1 */
        return 0;

    /* grow cache if needed */
    size_t need = (size_t)n;
    if (need > cache_cap) {
        size_t new_cap = need;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)         /* allocation failed */
            return 0;
        if (cache) {
            /* copy existing values */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    if (need > cache_len) {
        /* base values a(1)..a(15) */
        static const seq_t base[16] = {
            0ULL,      /* unused index 0 */
            4ULL, 16ULL, 56ULL, 250ULL, 984ULL,
            3888ULL, 15798ULL, 63092ULL, 252064ULL,
            1010554ULL, 4040556ULL, 16155482ULL,
            64620946ULL, 258358476ULL, 1032891644ULL
        };

        /* copy base values if not yet in cache */
        for (size_t i = cache_len + 1; i <= need && i <= 15; ++i)
            cache[i - 1] = base[i];

        /* recurrence for n >= 16 */
        for (size_t k = (cache_len < 16 ? 16 : cache_len + 1); k <= need; ++k) {
            seq_t val = 0;
            /* positive terms */
            val += ((seq_t)4) * cache[k - 2];
            val += ((seq_t)2) * cache[k - 3];
            val += ((seq_t)3) * cache[k - 4];
            /* negative terms */
            val -= ((seq_t)32) * cache[k - 5];
            val -= ((seq_t)34) * cache[k - 6];
            val -= ((seq_t)23) * cache[k - 7];
            val -= ((seq_t)158) * cache[k - 8];
            /* positive terms with larger lags */
            val += ((seq_t)295) * cache[k - 10];
            val += ((seq_t)42) * cache[k - 11];
            /* more negative terms */
            val -= ((seq_t)65) * cache[k - 12];
            /* positive terms */
            val += ((seq_t)12) * cache[k - 13];
            /* more negative terms */
            val -= ((seq_t)44) * cache[k - 14];
            /* positive terms */
            val += ((seq_t)2) * cache[k - 15];
            val += ((seq_t)24) * cache[k - 16];

            cache[k - 1] = val;
        }
        cache_len = need;
    }

    return cache[n - 1];
}
