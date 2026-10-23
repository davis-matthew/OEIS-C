/*
 * A043132  Numbers k such that 0 and 2 occur juxtaposed in the base-7
 *          representation of k but not of k-1.
 * https://oeis.org/A043132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that 0 and 2 occur juxtaposed in the base‑7 representation
   of k but not of k-1.  The function generates terms on demand and memoises
   them so that a(later) call for any n ≥ 1 returns the correct value. */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: return true if seq_t x has the adjacent digits 0‑2 or 2‑0 in
   its base‑7 representation. */
static inline bool has_adjacent_0_2(seq_t x)
{
    int prev = -1;             /* previous (more significant) digit */
    const seq_t seven = (seq_t)7;

    while (x > (seq_t)0) {
        int d = (int)(x % seven);
        if ((prev == 0 && d == 2) || (prev == 2 && d == 0))
            return true;
        prev = d;
        x /= seven;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – for safety */
        return (seq_t)0;

    /* Static cache of generated terms. */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    static size_t cached_cap = 0;
    static seq_t next_k = (seq_t)1;   /* next number to test */

    while (cached_len < (size_t)n) {
        /* Check current candidate. */
        if (has_adjacent_0_2(next_k) && !has_adjacent_0_2(next_k - (seq_t)1)) {
            /* Grow cache if necessary. */
            if (cached_len == cached_cap) {
                cached_cap = cached_cap ? cached_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, cached_cap * sizeof *cache);
            }
            cache[cached_len++] = next_k;
        }
        ++next_k;
    }

    return cache[(size_t)n - 1];
}
