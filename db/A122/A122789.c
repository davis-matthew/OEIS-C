/*
 * A122789  The (1,4)-entry in the matrix M^n, where M is the 4 X 4 matrix
 *          {{0, -1, -1, 1}, {1, -1, 0, 0}, {0, 1, 1, 0}, {0, 0, 1, 1 }}.
 * https://oeis.org/A122789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4); Initial values: a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=0
 * Range:     u8 n <= 22, u16 n <= 39, u32 n <= 74, u64 n <= 142,
 *            u128 n <= 280, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    size_t idx = (size_t)n;

    if (cache_size == 0) {
        cache_size = 4;
        cache = alloc_seq_array(cache_size);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 0;
    }

    if (idx < cache_size)
        return cache[idx];

    size_t old_size = cache_size;
    size_t new_size = idx + 1;
    if (new_size > cache_size) {
        seq_t *new_cache = alloc_seq_array(new_size);
        for (size_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    for (size_t i = old_size; i <= idx; ++i) {
        seq_t val = cache[i - 1];
        if (i >= 4)
            val += cache[i - 4];
        cache[i] = val;
    }

    return cache[idx];
}
