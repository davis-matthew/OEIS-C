/*
 * A044270  Numbers k such that string 2,1 occurs in the base 9 representation
 *          of k but not of k-1.
 * https://oeis.org/A044270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find numbers k such that the base‑9 representation of k contains
   the substring "21" but not the same substring in the representation
   of k-1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/seq_realloc.h"

/* Return true if the base‑9 representation of x contains the
   digit pair 2 followed by 1 (in the usual most‑significant‑first
   order).  When scanning from least significant to most significant
   this corresponds to seeing digits 1 then 2. */
static bool has21(seq_t x)
{
    seq_t prev = 9;               /* sentinel value larger than any digit 0..8 */
    while (x > 0) {
        seq_t digit = x % 9;
        if (prev == 1 && digit == 2)
            return true;
        prev = digit;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_k     = 1;          /* candidate to test */

    while (cache_len < (size_t)n) {
        /* Find the next k satisfying the condition. */
        seq_t k;
        for (;;) {
            k = (seq_t)next_k;
            seq_t km1 = (seq_t)(next_k - 1);
            if (has21(k) && !has21(km1))
                break;
            ++next_k;
        }

        /* Grow the cache if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache     = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = k;
        ++next_k;        /* move past the found k for the next search */
    }

    return cache[n - 1];
}
