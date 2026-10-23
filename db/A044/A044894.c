/*
 * A044894  Base 8 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base-8 representation has
 *            runs of equal digits with lengths alternating even, odd, even, …
 *            (the first run must have even length)
 * Range:     u8 n <= 28, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   valid_base8_runs_even_odd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/valid_base8_runs_even_odd.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t search_cur = 1;

    size_t target = (size_t)n;
    if (cache_len < target) {
        /* increase cache capacity if needed */
        if (cache_cap < target) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < target)
                new_cap *= 2;
            seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = newp;
            cache_cap = new_cap;
        }

        /* extend cache up to required index */
        while (cache_len < target) {
            if (valid_base8_runs_even_odd(search_cur))
                cache[cache_len++] = search_cur;
            ++search_cur;
        }
    }

    return cache[n - 1];
}
