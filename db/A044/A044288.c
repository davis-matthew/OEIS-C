/*
 * A044288  Numbers n such that string 4,1 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has41
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑9 representation of n contains the
   substring “41” but the base‑9 representation of n‑1 does not. */

/* Implementation uses a simple brute‑force search with memoisation.
   Because the density of qualifying numbers is about 1/81, the 10,000th term
   is well below one million, so the routine is comfortably fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has41.h"

/* Main function: a(n) as defined by OEIS A044288. */
seq_t a(idx_t n)
{
    /* Static memoisation storage. */
    static seq_t *vals = NULL;   /* array of found numbers */
    static size_t count = 0;     /* number of elements stored */
    static size_t capacity = 0;  /* allocated capacity */
    static seq_t last_checked = (seq_t)0;   /* last candidate examined */

    if (n == 0)   /* the sequence is undefined for n < 1; return 0 */
        return (seq_t)0;

    /* Grow storage if necessary. */
    if (vals == NULL) {
        capacity = 16;
        vals = (seq_t *)malloc(capacity * sizeof(seq_t));
    }

    /* Find elements until we have at least n of them. */
    while (count < n) {
        /* Advance candidate. */
        last_checked = last_checked + (seq_t)1;
        seq_t cand = last_checked;

        /* Check the property. */
        if (has41(cand) && !has41(cand - (seq_t)1)) {
            /* Store the value. */
            if (count == capacity) {
                capacity *= 2;
                vals = (seq_t *)realloc(vals, capacity * sizeof(seq_t));
            }
            vals[count++] = cand;
        }
    }

    /* Return the requested element. */
    return vals[n - 1];
}
