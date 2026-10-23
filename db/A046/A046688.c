/*
 * A046688  Antidiagonals of square array in which k-th row (k>0) is an
 *          arithmetic progression of difference 2^(k-1).
 * https://oeis.org/A046688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 + (d - k) * 2^(k-1), where d = floor((sqrt(8*n+1)-1)/2)
 *            (diagonal index), k = n - d*(d+1)/2 (offset within that
 *            diagonal). For k = 0 (row 0) a(n) = 1
 * Range:     u8 n <= 62, u16 n <= 186, u32 n <= 626, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the value at index n in OEIS sequence A046688. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that the first index of that diagonal
       (the cumulative sum of elements of previous diagonals) is <= n
       and cumulative + length of this diagonal > n. */
    idx_t d = 0;
    idx_t cum = 0;          /* total elements before current diagonal */
    while (cum + (d + 1) <= n)
    {
        cum += d + 1;       /* add length of diagonal d */
        d++;
    }

    /* offset within diagonal d. */
    idx_t off = n - cum;
    idx_t row = off;               /* row index (k) */
    idx_t col = d - off;           /* column index (m) */

    if (row == 0)
        return (seq_t)1;

    /* Compute 1 + col * 2^(row-1) */
    return (seq_t)1 + (seq_t)col * pow2(row - 1);
}
