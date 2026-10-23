/*
 * A044702  Numbers n such that string 7,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_77
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A044702 – n-th integer whose base‑9 representation contains
   the substring "77" while the base‑9 representation of n+1 does not.
   The function generates terms lazily and caches them, so calls can
   arrive in any order. */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_77.h"

/* Main sequence function.  idx_t is 1‑based; a(1) is the first term. */
seq_t a (idx_t n)
{
    /* Static cache of already generated terms. */
    static seq_t *terms = NULL;
    static size_t len   = 0;   /* number of terms stored     */
    static size_t cap   = 0;   /* allocated capacity         */
    static seq_t next   = 0;   /* next candidate to test     */

    if (n == 0)               /* undefined for n <= 0          */
        return (seq_t)0;

    /* Ensure we have at least n terms cached. */
    while (len < (size_t)n) {
        /* Find the next integer that satisfies the property. */
        for (seq_t x = next; ; ++x) {
            if (has_77(x) && !has_77(x + 1)) {
                /* Grow the cache if needed. */
                if (len == cap) {
                    size_t new_cap = cap ? cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                    if (!tmp) {          /* allocation failed – abort */
                        free(terms);
                        terms = NULL;
                        len = cap = 0;
                        return (seq_t)0;
                    }
                    terms = tmp;
                    cap   = new_cap;
                }
                terms[len++] = x;
                next = x + 1;          /* next search starts after this term */
                break;                 /* found the next required term   */
            }
        }
    }
    return terms[n - 1];
}
