/*
 * A104726  Triangle generated as the matrix product of A026729 and A000012
 *          (triangular views), read by rows.
 * https://oeis.org/A104726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A104726 – Triangle generated as the matrix product of A026729 and A000012
 *  and flattened by rows.
 *
 *  A(n) is obtained by first locating the triangle row r and column k that
 *  correspond to the flattened index n:
 *
 *          r = floor((sqrt(8*n+1)-1)/2)
 *          k = n - r*(r+1)/2            (0 ≤ k ≤ r)
 *
 *  Then
 *
 *          T(r,k) = Σ_{j=k..r} binomial(j, r-j)
 *
 *  and A(n) = T(r,k).
 *  This follows directly from the defining formula in the OEIS entry.
 */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t N, idx_t K) */

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((seq_t)r * ((seq_t)r + 1)) / 2 <= n)
        ++r;
    --r;

    /* Compute column k within that row */
    idx_t row_start = (idx_t)(((seq_t)r * ((seq_t)r + 1)) / 2);
    idx_t k = n - row_start;

    /* Sum binomial terms Σ_{j=k..r} binomial(j, r-j) */
    seq_t sum = 0;
    for (idx_t j = k; j <= r; ++j)
        sum += binom(j, r - j);

    return sum;
}
