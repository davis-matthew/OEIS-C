/*
 * A044096  Numbers n such that string 3,4 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for the n-th integer whose base‑5 representation contains
   the substring “34” but whose predecessor does not. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has34.h"

/* Return the n-th element of A044096 (n ≥ 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next = 1;          /* next candidate to test */

    while (cache_len < (size_t)n) {
        seq_t cand = next;
        bool cand_has = has34(cand);
        seq_t prev = cand - 1;
        bool prev_has = has34(prev);
        if (cand_has && !prev_has) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        ++next;
    }
    return cache[n - 1];
}
