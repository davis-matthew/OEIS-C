/*
 * A122872  Table by antidiagonals, T(n,k) is k-th number that starts with n
 *          in binary representation.
 * https://oeis.org/A122872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where the table T is defined by T(n,1) = n,
 *            T(n,2k) = 2·T(n,k), T(n,2k+1) = 2·T(n,k)+1. The sequence a(n)
 *            lists the values of this table in antidiagonal order (sum of
 *            indices constant, inner order by increasing row)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d (defined by row+col = d) that contains
       index n.  Diagonals are numbered starting with d = 2 for the
       first element (1,1).  Each diagonal d contains (d-1) elements. */
    idx_t sum   = 0;   /* cumulative elements before current diagonal   */
    idx_t step  = 1;   /* number of elements in the current diagonal   */

    while (sum + step < n) {
        sum  += step;
        ++step;
    }

    /* At exit: step = d-1,   sum = elements before diagonal d   */
    idx_t diag   = step + 1;              /* row + col = diag           */
    idx_t prev   = (step - 1) * step / 2; /* elements before diag d      */
    idx_t offset = n - prev;              /* 1‑based position inside diag */

    idx_t row = offset;                   /* row index (n in OEIS notation) */
    idx_t col = diag - offset;            /* column index (k)               */

    /* Compute T(row, col) using the closed formula:
       T(n, k) = k + (n-1) * 2^⌊log2(k)⌋                                   */
    /* floor(log2(col)) – col is always ≥ 1                               */
    idx_t fl = 63 - __builtin_clzll(col); /* works for 64‑bit idx_t           */
    seq_t pow2 = ((seq_t)1) << fl;       /* 2^⌊log2(k)⌋                      */
    seq_t result = (seq_t)col + (seq_t)(row - 1) * pow2;
    return result;
}
