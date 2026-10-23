/*
 * A044669  Numbers n such that string 4,1 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A044669.
   The sequence consists of the numbers n such that the base‑9
   representation of n contains the substring "41" (a digit 4
   immediately followed by a digit 1) while the base‑9 representation
   of n+1 does not.  The first term is 37.  The implementation
   builds a cache of found terms; subsequent calls are answered
   from the cache, so calls may arrive in any order. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if x contains the substring 4,1 in its base‑9
   representation, false otherwise. */
static bool has_41(seq_t x)
{
    seq_t prev = 9;            /* sentinel different from any digit 0–8 */
    while (x > 0) {
        seq_t d = x % 9;       /* least significant base‑9 digit */
        if (d == 4 && prev == 1) {
            return true;
        }
        prev = d;
        x = x / 9;
    }
    return false;
}

/* Exported function. */
seq_t a(idx_t n)
{
    /* Static cache of known terms. */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cap = 0;        /* capacity of cache */
    static idx_t count = 0;       /* number of terms stored (>=0) */

    /* If we have not computed enough terms, generate more. */
    if (count < n) {
        /* Start searching from the integer after the last cached
           value, or from 1 if nothing is cached yet. */
        seq_t cur = (count > 0) ? (cache[(size_t)count - 1] + 1) : 1;

        while (count < n) {
            /* Test the condition for cur. */
            if (has_41(cur) && !has_41(cur + 1)) {
                /* Expand the cache if necessary. */
                if (count >= cap) {
                    size_t new_cap = (cap == 0) ? 4 : cap << 1;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();   /* out of memory */
                    cache = tmp;
                    cap = new_cap;
                }
                cache[(size_t)count] = cur;
                ++count;
            }
            ++cur;  /* examine the next integer */
        }
    }

    /* Return the requested term. */
    return cache[(size_t)(n - 1)];
}
