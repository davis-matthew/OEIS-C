/*
 * A008345  a(n+1) = a(n)-b(n+1) if a(n) >= b(n+1) else a(n)+b(n+1), where
 *          {b(n)} are the triangular numbers A000217.
 * https://oeis.org/A008345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n+1) = (a(n) >= T(n+1)) ? a(n)-T(n+1) : a(n)+T(n+1),
 *            where T(k) = k*(k+1)/2
 * Range:     u8 n <= 15, u16 n <= 274, u32 n <= 7656, u64 n <= 7656,
 *            u128 n <= 7656, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static size_t cache_cap = 0;

    if (cache_size == 0) {
        ensure_capacity(1, &cache_cap, (void**)&cache, sizeof(seq_t));
        cache[0] = 0;
        cache_size = 1;
    }

    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t tri_idx = i;
        seq_t tri = (seq_t)tri_idx * ((seq_t)tri_idx + 1) / 2;
        cache[i] = (prev >= tri) ? prev - tri : prev + tri;
    }
    cache_size = n + 1;
    return cache[n];
}
