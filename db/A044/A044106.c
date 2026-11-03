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
 * Validated: standard, pass, 66 terms through n = 9602 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044106: numbers k such that the base‑6 representation of k
   contains the digraph 04 but the base‑6 representation of k‑1 does not. */

#include <stdbool.h>

/* Return true iff the base‑6 representation of x contains the digraph
   "04".  Digits are examined from least–significant to most‑significant. */
static bool has_04(seq_t x)
{
    seq_t prev = (seq_t)6;          /* sentinel value (6 > any base‑6 digit) */
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)6;     /* least‑significant digit */
        if (prev == (seq_t)4 && d == (seq_t)0)
            return true;            /* digraph found */
        prev = d;
        x /= (seq_t)6;
    }
    return false;
}

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
