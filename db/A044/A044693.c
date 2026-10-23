/*
 * A044693  Numbers n such that string 6,7 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   brute‑force search for numbers whose base‑9 representation *
 *            contains the substring "67" but whose successor n+1 does NOT
 *            contain it
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains67
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains67.h"

/* Main function required by the test harness */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;      /* array of found terms   */
    static idx_t  size  = 0;         /* current number of terms */
    static idx_t  cap   = 0;         /* capacity of terms array */
    static seq_t  next  = 1;         /* next candidate to test */

    /* Grow terms array if needed */
    if (size == 0 || size == cap) {
        cap = (cap > 0) ? 2 * cap : 16;
        terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        /* In practice the allocation will not fail for the ranges
           we need to support. */
    }

    /* Generate terms until we have at least n of them. */
    while (size < n) {
        /* Check condition for current candidate. */
        if (contains67(next) && !contains67(next + 1)) {
            terms[size++] = next;
        }
        ++next;
    }

    return terms[n - 1];
}
