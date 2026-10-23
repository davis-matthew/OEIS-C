/*
 * A158798  a(n) = a(n-1) + 64*a(n-2), a(0)=1, a(1)=8.
 * https://oeis.org/A158798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 64*a(n-2), a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) cache[i] = (seq_t)1;
            else if (i == 1) cache[i] = (seq_t)8;
            else
                cache[i] = cache[i-1] + (seq_t)64 * cache[i-2];
        }
        cache_len = new_len;
    }
    return cache[n];
}
