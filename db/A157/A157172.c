/*
 * A157172  Triangle, read by rows, T(n,k,m) = (m*(n-k)+1)*binomial(n-1, k-1)
 *          + (m*k+1)* binomial(n-1, k) - m*k*(n-k)*binomial(n-2, k-1), with m
 *          = 2.
 * https://oeis.org/A157172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) is the flattened triangle read by rows * T(n,k,2) =
 *            (2*(n-k)+1)*C(n-1,k-1) + (2*k+1)*C(n-1,k) - 2*k*(n-k)*C(n-2,k-1)
 *            * (0 ≤ k ≤ n). a(0)=1
 * Range:     i8 n <= 7, i16 n <= 111, i32 n <= 418, i64 n <= 1737,
 *            i128 n <= 5150, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t nn)
{
    /* Special case: the very first element */
    if (nn == 0)
        return (seq_t)1;

    /* 1. Determine the row number `r` and column index `k` (0‑based).    */
    /* row r has r+1 entries; the offset of row r is r*(r+1)/2.           */
    seq_t s = floor_sqrt((seq_t)8 * (seq_t)nn + (seq_t)1);   /* floor(sqrt(8*n+1)) */
    idx_t r   = (idx_t)((s - (seq_t)1) / (seq_t)2);         /* row number, 0‑based */
    idx_t start = r * (r + 1) / 2;                         /* first index of row r */
    idx_t k = nn - start;                                 /* column index in row r, 0‑based */

    /* 2. Compute the binomial coefficients, guarding against negative indices. */
    seq_t c1 = (k == 0) ? (seq_t)0 : binomial(r - 1, k - 1);
    seq_t c2 = binomial(r - 1, k);
    seq_t c3 = (r >= 2 && k > 0) ? binomial(r - 2, k - 1) : (seq_t)0;

    /* 3. Evaluate the three parts of the formula. */
    const seq_t M = (seq_t)2;
    seq_t term1 = ((M * (seq_t)(r - k) + (seq_t)1) * c1);
    seq_t term2 = ((M * (seq_t)k + (seq_t)1) * c2);
    seq_t term3 = (M * (seq_t)k * (seq_t)(r - k) * c3);

    /* 4. Combine them to obtain the sequence value. */
    return term1 + term2 - term3;
}
