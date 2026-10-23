/*
 * A044307  Numbers k such that the string 6,2 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that the base‑9 representation of k
   contains the adjacent digit pair 6,2 while the representation of k‑1
   does not.  The function finds the terms by scanning the natural
   numbers and caching the results.  This method is O(a(n)), which is
   sufficient for n up to at least 10 000. */

#include <stdbool.h>
#include <stdlib.h>   /* for malloc, realloc, abort */

static bool has62(seq_t value)
{
    /* Return true if the base‑9 representation of ‘value’ contains
       the adjacent digit pair 6,2.  Works for all non‑negative values. */
    if (value < 8)               /* less than two digits – impossible */
        return false;

    seq_t prev = value % 9;      /* least significant digit            */
    value /= 9;                  /* remaining higher digits             */

    while (value > 0) {
        seq_t curr = value % 9;
        if (curr == 6 && prev == 2)
            return true;
        prev = curr;
        value /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* cache of already found terms      */
    static size_t cap   = 0;       /* allocated capacity                */
    static size_t len   = 0;       /* number of terms stored            */
    static seq_t next_k = 1;       /* next integer to examine           */

    /* Grow the cache until it holds at least n terms. */
    if (len < n) {
        /* Resize the array if necessary.  Use a cast to allow C++ use. */
        if (cap < n) {
            size_t new_cap = (cap == 0) ? 4 : cap * 2;
            while (new_cap < n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof *terms);
            if (!tmp) {                /* out of memory; propagate error */
                abort();
            }
            terms = tmp;
            cap   = new_cap;
        }

        /* Generate terms until we have n of them. */
        while (len < n) {
            seq_t k = next_k;

            /* Ensure k > 0 before evaluating k-1. */
            if (k > 0 && has62(k) && !has62(k - 1))
                terms[len++] = k;

            ++next_k;
        }
    }

    return terms[n - 1];
}
