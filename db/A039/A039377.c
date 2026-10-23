/*
 * A039377  Numbers whose base-9 representation has the same nonzero number of
 *          2's and 8's.
 * https://oeis.org/A039377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   equal_2_and_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039377 – nth integer whose base‑9 representation has equal
   non‑zero counts of digit 2 and digit 8.  The function simply
   enumerates integers and caches those that satisfy the property. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/equal_2_and_8.h"

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t len   = 0;
    static size_t cap   = 0;
    static seq_t next   = (seq_t)1;   /* next integer to test */

    while (len < n) {
        /* Find the next integer that satisfies the property. */
        while (!equal_2_and_8(next))
            next = next + (seq_t)1;

        /* Resize cache if necessary. */
        if (len == cap) {
            size_t newcap = cap ? cap * 2 : 8;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();          /* allocation failure */
            cache = tmp;
            cap   = newcap;
        }

        cache[len++] = next;
        next = next + (seq_t)1;
    }

    return cache[n - 1];
}
