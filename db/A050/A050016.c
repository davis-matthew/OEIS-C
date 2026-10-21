/*
 * A050016  a(n) = floor(a(n-1)/2) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 9*a(n-1).
 * https://oeis.org/A050016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 821, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n>1,
   a(n) = floor(a(n-1)/2) if this value > 0 and not already in the sequence,
          otherwise a(n) = 9*a(n-1) */

#include <stdlib.h>

/* The function relies only on the arithmetic operators defined for seq_t
   and on the ability to compare two seq_t values with ==.  For the
   arbitrary‑precision build seq_t is a C++ class with the usual operators,
   while for the integer builds it is one of the standard unsigned types. */

seq_t a(idx_t n)
{
    /* Static cache: 1‑indexed sequence, 0 index unused. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t len   = 0;   /* number of initialized entries (len>=1) */

    /* Initialise the cache the first time the function is called. */
    if (cache == NULL) {
        /* Start with room for 8 terms; allocate two entries so that
           cache[1] is valid. */
        cap  = 8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out of memory: return 0 */
        len = 1;
        cache[1] = (seq_t)1;          /* a(1) = 1 */
    }

    /* Extend the cache up to the requested index n.  The sequence is
       computed lazily, so we may be called with n in any order. */
    while (len < n) {
        /* Compute candidate for the next term. */
        seq_t last = cache[len];
        seq_t cand = last / (seq_t)2;        /* floor division */

        /* Check if cand has already appeared in cache[1..len]. */
        int seen = 0;
        if (cand != (seq_t)0) {
            for (size_t i = 1; i <= len; ++i) {
                if (cache[i] == cand) { seen = 1; break; }
            }
        }

        /* Determine the next term. */
        seq_t next;
        if (seen || cand == (seq_t)0)
            next = last * (seq_t)9;   /* rule 2 */
        else
            next = cand;              /* rule 1 */

        /* Append the new term.  Grow the array if necessary. */
        if (len + 1 >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* out of memory */
            cache = tmp;
            cap = new_cap;
        }
        cache[++len] = next;
    }

    return cache[n];
}
