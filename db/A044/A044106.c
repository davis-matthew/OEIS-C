/*
 * A044106  Numbers k such that string 0,4 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 2034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_04
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044106: numbers k such that the base‑6 representation of k
   contains the digraph 04 but the base‑6 representation of k‑1 does not. */

#include <stdbool.h>
#include "../utils/has_04.h"

/* Sequence a(n),  n ≥ 1. */
seq_t a(idx_t n)
{
    /* Static storage for memoised terms.  Capacity 20000 is ample for
       n ≤ 10 000 and keeps the implementation independent of malloc. */
    static seq_t terms[20000];
    static idx_t term_cnt = 0;      /* number of stored terms */
    static idx_t next_k   = 0;      /* next integer candidate (k-1) */

    /* Generate terms until at least n of them are available. */
    while (term_cnt < n) {
        seq_t k = (seq_t)(next_k + 1);          /* candidate integer */
        if (has_04(k) && !has_04(k - (seq_t)1)) {
            terms[term_cnt++] = k;              /* store the term */
        }
        next_k++;                               /* advance to next k */
    }

    return terms[n - 1];   /* a(1) is the first stored element */
}
