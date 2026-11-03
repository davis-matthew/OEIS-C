/*
 * A008543  Sextuple factorial numbers: Product_{k=0..n-1} (6*k + 5).
 * https://oeis.org/A008543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=0..n-1} (6*k+5)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 297 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache == NULL) {
        cache_size = 1;
        cache = mem_new(seq_t, 1);
        if (cache == NULL) abort();
        cache[0] = (seq_t)1;
    }

    if (n < cache_size)
        return cache[n];

    idx_t new_size = n + 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_size);
    if (new_cache == NULL) abort();

    cache = new_cache;
    for (idx_t i = cache_size; i < new_size; ++i) {
        seq_t mul = (seq_t)(6 * i - 1);   /* multiplier for a(i) */
        cache[i] = mul * cache[i - 1];
    }
    cache_size = new_size;
    return cache[n];
}
