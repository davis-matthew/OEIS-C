/*
 * A044263  Numbers k such that string 1,3 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the n‑th integer k such that the digit pattern 1,3
   appears in the base‑9 representation of k but not in that of k‑1.
   The sequence is defined by scanning consecutive integers and keeping
   those that satisfy the condition. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/* Return true iff seq_t x contains the substring "13" in its base‑9 form.
   The digits are extracted by repeated division by 9. */
static bool has13(seq_t x)
{
    if (x == 0)
        return false;

    uint8_t digs[64];
    size_t len = 0;

    while (x > 0) {
        digs[len++] = (uint8_t)(x % 9);
        x /= 9;
    }

    for (size_t i = len; i > 1; --i) {
        if (digs[i - 1] == 1 && digs[i - 2] == 3)
            return true;
    }
    return false;
}

/* a(n) returns the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static seq_t next_k = 1;          /* Start scanning from k = 1 */

    while (cache_len < n) {
        /* Find the next integer satisfying the condition. */
        while (!(has13(next_k) && !has13(next_k - 1)))
            ++next_k;

        /* Ensure capacity for the cache. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = seq_realloc(cache, cache_cap, new_cap);
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Store the value and advance. */
        cache[cache_len++] = next_k;
        ++next_k;
    }

    return cache[n - 1];
}
