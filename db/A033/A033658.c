/*
 * A033658  Trajectory of 25 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + reverse_decimal(a(n-1)), a(0) = 25
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   reverse_decimal, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/reverse_decimal.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;

    if (cache == NULL) {
        cache_cap = 1;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)25;
        cache_len = 1;
    }

    if (n == 0)
        return cache[0];

    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t rev = reverse_decimal(prev);
        cache[i] = prev + rev;
    }
    cache_len = n + 1;
    return cache[n];
}
