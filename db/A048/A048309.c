/*
 * A048309  Numbers whose base-12 expansions, read from left to right, have
 *          run lengths that strictly increase.
 * https://oeis.org/A048309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer whose base‑12 run lengths strictly increase
 * Range:     u8 n <= 31, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   base12_increasing_runs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/base12_increasing_runs.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    static seq_t   next_val  = 0;

    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure – aborting, as this is fatal */
            abort();
        }
    }

    while (cache_len <= (size_t)n) {
        if (base12_increasing_runs(next_val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) {
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_val;
        }
        ++next_val;
    }

    return cache[(size_t)n];
}
