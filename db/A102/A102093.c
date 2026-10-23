/*
 * A102093  Slowest increasing sequence where the digits, taken one by one,
 *          show the pattern odd/odd/even/even/odd/odd/even/even... Only the
 *          terms < 300 differ from A098161.
 * https://oeis.org/A102093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   match_and_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102093
   Defined as: a(1)=1.
   For n>1, a(n) is the smallest integer >a(n‑1) such that when all terms
   up to a(n) are concatenated, the decimal digits follow the repeating
   pattern odd, odd, even, even, odd, odd, … (period 4).  The pattern
   continues across term boundaries.  The function computes values on demand
   and caches them so that calls can be in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/match_and_len.h"

/* Computes a(n) for n>=1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe */

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;
    static seq_t last_value = 0;
    static uint8_t pattern_pos = 0;   /* next required pattern position */

    /* Return cached value if available. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow cache until we have at least n terms. */
    while (cache_size < n) {
        /* Expand physical storage if needed. */
        if (cache_size >= cache_capacity) {
            size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_capacity = new_cap;
        }

        /* Find the next valid term. */
        seq_t cand = last_value + 1;
        while (true) {
            int len = 0;
            if (match_and_len(cand, pattern_pos, &len)) {
                /* Accept the candidate. */
                cache[cache_size++] = cand;
                last_value = cand;
                pattern_pos = (uint8_t)((pattern_pos + (uint8_t)len) & 3);
                break;
            }
            cand++;
        }
    }

    return cache[n - 1];
}
