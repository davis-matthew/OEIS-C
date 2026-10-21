/*
 * A228158  Numbers n such that the cardinality of (natural numbers <=n with a
 *          first digit of 1) = n/2.
 * https://oeis.org/A228158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 2 repeated ((n+1)/2) times for odd n; a(n) = a(n-1)*8 for
 *            even n
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Cache of previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of valid indices in cache */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n < 1)
        return (seq_t)0;   /* undefined for n == 0, guard */

    /* Ensure we have enough room to store up to index n. */
    if ((size_t)n + 1 > cache_cap)
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache,
                        sizeof(seq_t));

    /* Compute any missing values up to n. */
    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            if (i == 0) {
                /* Index 0 is unused. */
                continue;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else if ((i & 1) == 1) {   /* odd n */
                idx_t digits = (i + 1) / 2;
                seq_t val = 0;
                for (idx_t j = 0; j < digits; ++j)
                    val = val * (seq_t)10 + (seq_t)2;
                cache[i] = val;
            } else {                     /* even n */
                cache[i] = cache[i-1] * (seq_t)8;
            }
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
