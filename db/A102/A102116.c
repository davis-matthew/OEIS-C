/*
 * A102116  Iccanobirt numbers (6 of 15): a(n) = R(a(n-1)) + R(a(n-2)) +
 *          a(n-3), where R is the digit reversal function A004086.
 * https://oeis.org/A102116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = R(a(n‑1)) + R(a(n‑2)) + a(n‑3) (n ≥ 3) with initial
 *            values a(0)=0, a(1)=0, a(2)=1. R(x) is the decimal
 *            digit‑reversal of x
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 31, u64 n <= 60, u128 n <= 108,
 *            big any n
 * Helpers:   rev_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/rev_seq.h"

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *tbl = NULL;   /* array of computed values */
    static idx_t  tbl_size = 0; /* number of values currently stored */
    static idx_t  tbl_cap  = 0; /* allocated capacity */

    /* initialise with the first three terms */
    if (!tbl) {
        tbl_cap   = 3;
        tbl       = (seq_t *)malloc(tbl_cap * sizeof(seq_t));
        tbl[0] = 0;
        tbl[1] = 0;
        tbl[2] = 1;
        tbl_size = 3;
    }

    /* grow the table if we need to access beyond the current capacity */
    if (n >= tbl_cap) {
        idx_t new_cap = tbl_cap;
        while (new_cap <= n)
            new_cap <<= 1;                /* double until we have enough */
        seq_t *tmp = (seq_t *)realloc(tbl, new_cap * sizeof(seq_t));
        if (!tmp)                       /* allocation failed – return 0 */
            return 0;
        tbl = tmp;
        tbl_cap = new_cap;
    }

    /* compute any missing values up to n */
    if (n >= tbl_size) {
        for (idx_t i = tbl_size; i <= n; ++i) {
            seq_t rev1 = rev_seq(tbl[i-1]);
            seq_t rev2 = rev_seq(tbl[i-2]);
            tbl[i] = rev1 + rev2 + tbl[i-3];
        }
        tbl_size = n + 1;
    }

    return tbl[n];
}
