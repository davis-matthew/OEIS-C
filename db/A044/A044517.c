/*
 * A044517  Numbers n such that string 5,4 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   append_candidate, match54_in_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sequential search for numbers n where the base‑6 representation
 * contains the substring "54" and every such occurrence is followed only by
 * digits 5.  The base‑6 representation of n+1 then contains no "54". */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/match54_in_base6.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t next = 1;

    while (cache_len < n) {
        while (!match54_in_base6(next)) ++next;
        append_candidate(&cache, &cache_len, &cache_cap, (seq_t)next);
        ++next;
    }
    return cache[n - 1];
}
