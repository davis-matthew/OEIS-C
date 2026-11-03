/*
 * A061814  Multiples of 4 containing only even digits.
 * https://oeis.org/A061814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 973 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth member of OEIS A061814:
   the multiples of 4 whose decimal digits are all even (0,2,4,6,8).
   It generates multiples of 4 in increasing order, keeps those whose
   digits are all even, and caches them so any call to a(n) returns
   the correct term regardless of order of calls. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether all decimal digits of x are even. */
static inline bool all_even_digits(seq_t x)
{
    if (x == 0)           /* 0 is considered to have only even digits */
        return true;

    while (x != 0) {
        seq_t dig   = x % 10;     /* least significant decimal digit   */
        if ((dig % 2) != 0)       /* odd digit found                    */
            return false;
        x /= 10;                  /* move to next digit                  */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* local static cache and state */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;
    static seq_t next_mult = 0;   /* next multiple of 4 to examine */

    if (!cache) {
        cap   = 64;
        cache = mem_new(seq_t, cap);
    }

    /* extend cache until we have computed a(n) */
    while (len <= (size_t)n) {
        seq_t val = next_mult;
        if (all_even_digits(val)) {
            if (len >= cap) {
                cap <<= 1;   /* double capacity */
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[len++] = val;
        }
        next_mult += (seq_t)4;
    }

    return cache[(size_t)n];
}
