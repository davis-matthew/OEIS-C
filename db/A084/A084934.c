/*
 * A084934  Rectangular array T(m,n) (m>=1, n>=1) read by antidiagonals: row m
 *          consists of the numbers ( i + mj : i >= 0, j >= 0 ), sorted in
 *          increasing order, with repetitions allowed.
 * https://oeis.org/A084934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at the antidiagonal position (m,n) in the
 *            rectangular array whose row m consists of all numbers i + m*j
 *            (i,j ≥ 0) sorted in non‑decreasing order with repetitions. The
 *            index n of the sequence is obtained by reading the array by
 *            antidiagonals (diagonal d = i + j–1). The internal helper...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   T_of_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t used in idx_t */
#include "../utils/T_of_row.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d containing position n.
       The total number of entries up to diagonal d is d*(d+1)/2. */
    idx_t d = 0;
    while ((d * (d + 1) / 2) < n)
        ++d;

    /* Indices inside the diagonal:
       previous diagonals contain prev = (d-1)*d/2 entries,
       the position inside the current diagonal is s (1‑based). */
    idx_t prev = ((d - 1) * d) / 2;
    idx_t s = n - prev;          /* 1‑based index inside diagonal */

    /* Coordinates in the array: row = s, column = d + 1 - s. */
    idx_t row = s;
    idx_t col = d + 1 - s;

    return T_of_row(row, col);
}
