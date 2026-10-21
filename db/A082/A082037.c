/*
 * A082037  A square array of linear-factorial numbers, read by antidiagonals.
 * https://oeis.org/A082037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 51, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A082037: square array of linear‑factorial numbers, read by antidiagonals.
    T(n,k) = (k*n + 1)*n!  (n = column, k = row)
    The  sequence a(n) is obtained by enumerating the array in antidiagonals
    with increasing n (columns) for each diagonal.  */
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* Find the diagonal index s such that
       C_s = (s+1)(s+2)/2 > n  (C_s is the cumulative count of terms
       up to and including diagonal s).  */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n) {
        ++s;
    }

    /* Previous diagonal cumulative count */
    idx_t prev = s * (s + 1) / 2;

    /* Position of n within the current diagonal (0‑based) */
    idx_t pos = n - prev;           /* 0 ≤ pos ≤ s */
    idx_t col = pos;                /* column index (n in T(n,k)) */
    idx_t row = s - pos;            /* row index (k in T(n,k)) */

    /* Compute term: T(col,row) = (row*col + 1) * col!  */
    seq_t fact = factorial(col);            /* col!  */
    seq_t factor = (seq_t)row * (seq_t)col + 1; /* (row*col + 1) */
    return fact * factor;
}
