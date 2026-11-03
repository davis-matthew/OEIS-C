/*
 * A033650  Trajectory of 7 under map x --> x + (x-with-digits-reversed).
 * https://oeis.org/A033650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 44, u128 n <= 102,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 988 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 7, a(n) = a(n−1) + reverse_decimal(a(n−1)) for n > 0 */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/reverse_digits.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

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
        seq_t rev  = reverse_digits(prev);
        cache[cache_len] = prev + rev;
        ++cache_len;
    }

    return cache[n];
}
