/*
 * A183821  1/256 the number of (n+1) X 3 0..3 arrays with no 2 X 2 subblock
 *          being a reflection across the shared element pair of any
 *          horizontal or vertical neighbor.
 * https://oeis.org/A183821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) + 645*a(n-2) - 2451*a(n-3) Initial values: a(1)
 *            = 15, a(2) = 813, a(3) = 43947
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return 0; /* undefined for 0, return 0 */

    if (cache == NULL || n >= cache_size) {
        size_t new_size = (size_t)(n + 1); /* indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure; fall back to computing in local array */
            new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            if (!new_cache)
                return 0; /* unable to allocate, return 0 */
        }
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 0; /* unused */
            } else if (i == 1) {
                val = 15;
            } else if (i == 2) {
                val = 813;
            } else if (i == 3) {
                val = 43947;
            } else {
                seq_t a1 = cache[i - 1];
                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];

                /* 43*a1  = (a1<<5) + (a1<<3) + (a1<<1) + a1   */
                seq_t temp1 = (a1 << 5) + (a1 << 3) + (a1 << 1) + a1;

                /* 645*a2 = (a2<<9) + (a2<<7) + (a2<<2) + a2   */
                seq_t temp2 = (a2 << 9) + (a2 << 7) + (a2 << 2) + a2;

                /* 2451*a3 = (a3<<11) + (a3<<8) + (a3<<7)
                             + (a3<<4) + (a3<<1) + a3          */
                seq_t temp3 = (a3 << 11) + (a3 << 8) + (a3 << 7)
                            + (a3 << 4) + (a3 << 1) + a3;

                val = temp1 + temp2 - temp3;
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
