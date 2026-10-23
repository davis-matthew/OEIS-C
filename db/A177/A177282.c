/*
 * A177282  Number of permutations of 2 copies of 1..n with all adjacent
 *          differences <= 1 in absolute value.
 * https://oeis.org/A177282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=6, a(3)=12, a(4)=26, a(5)=48; a(n) =
 *            3*a(n-1) - 2*a(n-2) - a(n-3) + a(n-4) for n >= 6
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    if (n >= cache_sz) {
        idx_t new_sz = n + 1;
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        for (idx_t i = cache_sz; i < new_sz; ++i) {
            switch (i) {
                case 0: cache[i] = (seq_t)1; break;
                case 1: cache[i] = (seq_t)1; break;
                case 2: cache[i] = (seq_t)6; break;
                case 3: cache[i] = (seq_t)12; break;
                case 4: cache[i] = (seq_t)26; break;
                case 5: cache[i] = (seq_t)48; break;
                default:
                    cache[i] = 3 * cache[i-1]
                               - 2 * cache[i-2]
                               -      cache[i-3]
                               +      cache[i-4];
            }
        }
        cache_sz = new_sz;
    }
    return cache[n];
}
