/*
 * A062057  Numbers with 7 odd integers in their Collatz (or 3x+1) trajectory.
 * https://oeis.org/A062057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 152, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has7_odds
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose Collatz trajectory contains exactly 7 odd integers */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has7_odds.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t len = 0;
    static idx_t cap = 0;
    static idx_t next_start = 1;

    /* extend cache until it contains at least n terms */
    while (len < n) {
        /* find next candidate with exactly 7 odd Collatz terms */
        while (!has7_odds((seq_t)next_start))
            next_start++;

        /* resize cache if needed */
        if (len == cap) {
            idx_t new_cap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t*)malloc((size_t)new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* allocation failed */
            for (idx_t i = 0; i < len; ++i)
                tmp[i] = cache[i];
            if (cache) free(cache);
            cache = tmp;
            cap = new_cap;
        }

        cache[len++] = (seq_t)next_start;
        next_start++;
    }

    return cache[n - 1];
}
