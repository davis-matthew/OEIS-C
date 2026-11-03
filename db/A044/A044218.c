/*
 * A044218  Numbers k such that string 3,7 occurs in the base 8 representation
 *          of k but not of k-1.
 * https://oeis.org/A044218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 9875 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the base‑8 representation of k contains the
 * substring "37" but the representation of k-1 does not.
 *  The function returns the n‑th member of the sequence (offset 1). */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper that returns true iff the octal representation of x contains
 * the adjacent digits 3 followed by 7 (most significant → least).   */
static bool has37_oct(seq_t x)
{
    if (x == 0) return false;          /* 0 has no digits 3,7 */
    seq_t prev = x % 8;                /* least significant digit */
    x /= 8;
    while (x > 0) {
        seq_t digit = x % 8;
        if (digit == 3 && prev == 7)   /* pattern 3,7 detected */
            return true;
        prev = digit;
        x /= 8;
    }
    return false;
}

/* Return the n‑th term of OEIS A044218.  The function is memoised so that
 * repeated calls are fast even when n is supplied in arbitrary order. */
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache = NULL;
    static idx_t cached_terms = 0;
    static idx_t capacity = 0;
    static seq_t next_k = 1;                /* next candidate to test */

    /* Ensure cache large enough to store *at least* n terms */
    if (cached_terms < n) {
        /* Increase capacity until it holds at least n */
        while (cached_terms < n) {
            /* Grow cache if needed */
            if (cached_terms == capacity) {
                size_t new_cap = capacity ? capacity * 2 : 128;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                cache = new_cache;
                capacity = new_cap;
            }

            /* Test current_k */
            seq_t curr = next_k;
            bool cur_has = has37_oct(curr);
            bool prev_has = has37_oct(curr - 1);
            if (cur_has && !prev_has) {
                cache[cached_terms++] = curr;
            }
            ++next_k;               /* next candidate */
        }
    }
    return cache[n - 1];
}
