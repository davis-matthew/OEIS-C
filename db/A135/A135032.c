/*
 * A135032  a(n) = 6*a(n-1) + 4*a(n-2).
 * https://oeis.org/A135032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0; a(1)=1; a(n)=6*a(n-1)+4*a(n-2) for n>=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
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

    /* initialise cache with base terms if needed */
    if (cache_size == 0) {
        cache_size = 2;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* extend cache to include term n */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out‑of‑memory guard */

        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else {
                cache[i] = (seq_t)6 * cache[i-1] + (seq_t)4 * cache[i-2];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
