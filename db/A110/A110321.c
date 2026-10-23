/*
 * A110321  A Jacobsthal number related number triangle.
 * https://oeis.org/A110321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) with r = row_of(n) and k = n - r*(r+1)/2 T(r,k) =
 *            r! * J(r - k + 1) / k! where J(m) is the Jacobsthal sequence:
 *            J(0)=0, J(1)=1, J(m)=J(m-1)+2*J(m-2)
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   jacobsthal, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the utility that provides row_of */
#include "../utils/row_of.h"
#include "../utils/jacobsthal.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row r and column k */
    idx_t r = row_of(n);                               /* 0‑based row */
    idx_t k = n - r * (r + 1) / 2;                     /* 0‑based column */

    /* Compute product r! / k!  (product of (k+1) .. r) */
    seq_t prod = 1;
    for (idx_t i = k + 1; i <= r; ++i)
        prod *= (seq_t)i;

    /* Jacobsthal index: r - k + 1 */
    idx_t jacob_idx = r - k + 1;
    seq_t j = jacobsthal(jacob_idx);

    return prod * j;
}
