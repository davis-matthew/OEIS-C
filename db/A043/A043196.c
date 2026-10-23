/*
 * A043196  Numbers k such that 2 and 4 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th number k (offset 1) such that the
 * base‑9 representation of k contains the digits 2 and 4 next to each
 * other in either order, but the representation of k‑1 does not.
 * The sequence is generated incrementally and cached, so subsequent
 * calls require only the new terms to be computed. */

#include <stdlib.h>
#include <stdbool.h>

static bool has_adjacent_2_4(seq_t x)
{
    seq_t prev = 0;
    while (x > 0) {
        seq_t curr = x % 9;
        /* Look for "24" or "42" in the next two most significant digits.
         * With the least‑significant digit first, the pattern "24" appears
         * as curr == 2 && prev == 4, and "42" as curr == 4 && prev == 2.   */
        if ((curr == 2 && prev == 4) || (curr == 4 && prev == 2))
            return true;
        prev = curr;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The sequence begins at a(1).  n is guaranteed to be >= 1.        */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k = 1;          /* next candidate to test            */

    size_t target = (size_t)n;        /* target number of terms            */

    if (!cache) {                     /* initialise cache on first call    */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Generate terms until we have enough for the requested n.       */
    while (cache_len < target) {
        /* Find the next k that satisfies: has_adjacent_2_4(k) &&
         * !has_adjacent_2_4(k-1).                                      */
        while (!has_adjacent_2_4(next_k) || has_adjacent_2_4(next_k - 1))
            ++next_k;

        /* Grow the cache if necessary.                                 */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        cache[cache_len++] = next_k;
        ++next_k;
    }

    return cache[target - 1];
}
