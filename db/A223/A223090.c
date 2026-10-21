/*
 * A223090  Numbers k with the property that if the base-8 representation of k
 *          is read backwards, the result is an integral multiple of k.
 * https://oeis.org/A223090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer k such that the base‑8
 *            representation of k read backwards is an integer multiple of k
 *            (i.e. rev_8(k) % k == 0 and rev_8(k)/k > 1). The resulting terms
 *            are all multiples of 7; the first few are listed in the OEIS
 *            entry
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   rev_oct
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/rev_oct.h"

/* a(n) ------------------------------------------------------------*/

seq_t a (idx_t n)
{
    /* static memoization table ---------------------------------- */
    static seq_t *terms = NULL;      /* array of found terms */
    static size_t size  = 0;          /* number of terms stored  */
    static size_t cap   = 0;          /* capacity of the array   */
    static idx_t  last  = 0;          /* last multiple of 7 examined */

    /* initialise the dynamic array on first call */
    if (terms == NULL) {
        cap   = 128;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
    }

    /* generate terms up to the requested index */
    while (size < (size_t)n) {
        last += 7;                      /* only multiples of 7 need be examined */

        seq_t r = rev_oct(last);
        /* we need a proper multiple (ratio > 1) */
        if (r != last && r % last == 0) {
            /* add the term to the cache */
            if (size == cap) {
                cap <<= 1;
                terms = (seq_t*)realloc(terms, cap * sizeof(seq_t));
            }
            terms[size++] = last;
        }
    }

    return terms[n - 1];
}
