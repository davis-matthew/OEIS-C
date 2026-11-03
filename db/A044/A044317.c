/*
 * A044317  Numbers n such that string 7,3 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044317: numbers n such that the base‑9 representation of n
 * contains the substring 73 but the base‑9 representation of n‑1 does not. */
/* The implementation searches for these numbers, caching results to
 * support calls in any order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑9 representation of x contains the substring 73. */
static bool contains73(seq_t x)
{
    if (x == 0)
        return false;          /* base‑9 is "0" – cannot contain "73" */

    seq_t t = x;
    int prev = -1;              /* previous (less‑significant) digit */
    while (t > 0)
    {
        int d = (int)(t % 9);   /* current least‑significant digit */
        /* The substring "73" appears when a more‑significant digit is 7
         * followed by a less‑significant digit 3. In the loop we see the
         * less‑significant digit first (prev) and then the more‑significant
         * digit (d). Thus we test d==7 && prev==3. */
        if (d == 7 && prev == 3)
            return true;
        prev = d;
        t = t / 9;
    }
    return false;
}

/* Return the n-th term of the sequence, n ≥ 1. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* array of found terms */
    static size_t cap   = 0;       /* allocated capacity */
    static idx_t  len   = 0;       /* number of terms stored */
    static seq_t next_val = 1;     /* next candidate to test */

    /* Grow the cache until it contains the required term. */
    while (len < n)
    {
        bool has      = contains73(next_val);
        bool hasPrev  = contains73(next_val - (seq_t)1);

        if (has && !hasPrev)
        {
            /* Ensure space for another element. */
            if (len == cap)
            {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    exit(EXIT_FAILURE);
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = next_val;
        }

        next_val = next_val + (seq_t)1;
    }

    return cache[n - 1];
}
