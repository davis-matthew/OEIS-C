/*
 * A033650  Trajectory of 7 under map x --> x + (x-with-digits-reversed).
 * https://oeis.org/A033650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 102,
 *            big any n
 * Helpers:   reverse_decimal, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 7, a(n) = a(n−1) + reverse_decimal(a(n−1)) for n > 0 */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/reverse_decimal.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;
    static size_t cache_cap = 0;

    /* Initialize cache with the first term */
    if (!cache) {
        cache_cap = 1;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)7;
        cache_len = 1;
    }

    /* Grow cache until it reaches the required index */
    while (cache_len <= n) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }
        seq_t prev = cache[cache_len - 1];
        seq_t rev  = reverse_decimal(prev);
        cache[cache_len] = prev + rev;
        ++cache_len;
    }

    return cache[n];
}
