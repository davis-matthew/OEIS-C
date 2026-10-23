/*
 * A038354  Numbers whose base-6 representation has the same nonzero number of
 *          2's and 5's.
 * https://oeis.org/A038354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   has_equal_2_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038354: the n‑th positive integer whose base‑6 representation
   contains the same (non‑zero) number of 2’s and 5’s. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_2_5.h"

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* static state to keep a cache of already found terms */
    static seq_t *cache = NULL;     /* array of found terms */
    static size_t   cap   = 0;      /* allocated capacity of cache */
    static idx_t    found = 0;      /* number of terms stored so far */
    static seq_t   next  = 0;       /* next candidate to examine */

    /* generate terms until we have at least n of them */
    while (found < n) {
        ++next;                      /* examine the next integer */
        if (has_equal_2_5(next)) {
            if (found >= cap) {      /* grow the cache if necessary */
                size_t new_cap = cap ? cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failed – give up */
                cache = tmp;
                cap   = new_cap;
            }
            cache[found++] = next;   /* store the term */
        }
    }
    return cache[n-1];
}
