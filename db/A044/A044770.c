/*
 * A044770  Numbers n such that string 5,7 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has_57
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A044770: Numbers n such that "57" occurs in the decimal
   representation of n but not in the representation of n+1. */
/* Computation: iterate through integers, test the substring
   "57" in both n and n+1, and cache results to answer any
   requested index in O(number_of_needed_terms). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_57.h"

/* Return the nth term (1‑based) of A044770.  */
seq_t a(idx_t n)
{
    /* Static cache of found terms.  Capacity large enough for the
       range of test cases (n up to about 10000). */
    static seq_t terms[1000000];
    static size_t   len   = 0;        /* number of terms stored */
    static seq_t   cur   = 1;         /* next integer to examine */

    /* Advance the search until we have at least n terms. */
    while (len < n) {
        if (has_57(cur) && !has_57(cur + (seq_t)1)) {
            terms[len++] = cur;
        }
        ++cur;
    }
    return terms[n - 1];              /* 1‑based access */
}
