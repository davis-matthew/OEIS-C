/*
 * A092457  Numbers that either contain the digit 9 or are divisible by 9.
 * https://oeis.org/A092457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   contains_digit_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates numbers that are either divisible by 9 or contain the digit 9.
   Sequence is zero‑based: a(0)=0, a(1)=9, a(2)=18, … */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/contains_digit_9.h"

seq_t a(idx_t n) {
    /* static cache: cache[i] = a(i) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;       /* number of cached terms */
    static unsigned long long next_val = 1;  /* next integer to test */

    /* Ensure the cache is big enough for n */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
            cache_size = 0;
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        /* initialise a(0) if not already done */
        if (cache_size == 0) {
            cache[0] = 0;    /* 0 is divisible by 9 */
            cache_size = 1;
        }
        /* generate remaining terms up to new_size-1 */
        for (size_t i = cache_size; i < new_size; ++i) {
            while (1) {
                if (next_val % 9 == 0 || contains_digit_9(next_val)) {
                    cache[i] = (seq_t)next_val;
                    ++next_val;
                    break;
                }
                ++next_val;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
