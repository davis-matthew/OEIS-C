/*
 * A044603  Numbers n such that string 4,3 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS A044603.  In OEIS notation: */
/*  a(n) = the nth integer ≥ 1 such that the base‑8 representation of */
/*  n contains the consecutive digits 4,3 while the representation of n+1 */
/*  does not.  The sequence has offset 1. */

/* The function is memoised; it can be called in any order and will return */
/* the correct 64‑bit (or arbitrary precision) value for all n ≥ 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

/* Return true if the octal representation of x contains the pattern 4,3. */
static bool has43(seq_t x)
{
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 8);
        if (prev == 3 && d == 4)
            return true;
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static idx_t cache_size = 0; /* number of cached terms   */
    static idx_t cache_cap   = 0; /* capacity of the array    */
    static seq_t candidate = 1;   /* next integer to examine */

    /* Grow the cache until it contains at least n terms. */
    while (cache_size < n) {
        bool found = false;
        while (!found) {
            if (has43(candidate) && !has43(candidate + 1)) {
                /* Ensure we have room for the new term. */
                if (cache_size == cache_cap) {
                    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_size++] = candidate;
                found = true;
            }
            ++candidate;
        }
    }
    return cache[n - 1];
}
