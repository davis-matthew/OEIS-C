/*
 * A117204  Squarefree positive integers k such that 2*k+1 is also squarefree.
 * https://oeis.org/A117204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117204: Squarefree positive integers k such that 2*k+1 is also squarefree.
   Computed by a simple search using the helper function is_squarefree().
   The implementation keeps a memoized list of already found terms so that
   any order of calls with n ≥ 1 is efficient. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *terms = NULL;        /* array of found terms */
    static size_t  capacity = 0;       /* current allocated capacity */
    static idx_t   count    = 0;       /* number of terms stored */
    static idx_t   last_checked = 0;   /* integer k that has been examined last */

    /* ensure we have an array ready */
    if (terms == NULL) {
        capacity = 16;
        terms = (seq_t*)realloc(NULL, capacity * sizeof(seq_t));
    }

    /* extend array if needed */
    if ((size_t)(n - 1 + 1) > capacity) {
        while (capacity < (size_t)(n))
            capacity *= 2;
        terms = (seq_t*)realloc(terms, capacity * sizeof(seq_t));
    }

    /* generate new terms until we have at least n terms */
    while (count < n) {
        last_checked++;
        seq_t k = (seq_t)last_checked;

        if (is_squarefree(k) && is_squarefree(k * (seq_t)2 + (seq_t)1)) {
            terms[count++] = k;
        }
    }

    return terms[n - 1];
}
