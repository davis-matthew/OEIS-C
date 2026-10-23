/*
 * A131035  Triangle read by rows: A051340 * A129686 as infinite lower
 *          triangular matrices.
 * https://oeis.org/A131035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 1325, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   _ij_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A131035: flattened entry of the product of A051340 and A129686 as lower triangular matrices.
   The triangle entry T(i,j) for i ≥ j ≥ 0 is given by
        T(i,j) = (i-j+1)
                + (i>=2 && j<=i-2 ? (i-j-1) : 0)
                - (i>=2 && j>=1   ? (i-j   ) : 0)
                - (i>=3 && j<=i-3 ? (i-j-2) : 0)
   The flattened index is a(n) = T(i,j) where i is the row number and j the column
   obtained from n via the standard mapping n = i(i+1)/2 + j.  */
#include <stdint.h>
#include <stddef.h>
#include "../utils/_ij_from_n.h"

/* Compute triangle entry T(i,j) (i≥j) */
static seq_t _T(idx_t i, idx_t j)
{
    seq_t val = (seq_t)(i - j + 1);

    if (i >= 2 && j <= i - 2)
        val += (seq_t)(i - j - 1);

    if (i >= 2 && j >= 1)
        val -= (seq_t)(i - j);

    if (i >= 3 && j <= i - 3)
        val -= (seq_t)(i - j - 2);

    return val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    _ij_from_n(n, &i, &j);
    return _T(i, j);
}
