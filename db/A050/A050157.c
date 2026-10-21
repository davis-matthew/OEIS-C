/*
 * A050157  T(n, k) = S(2n, n, k) for 0<=k<=n and n>=0, where S(p, q, r) is
 *          the number of upright paths from (0, 0) to (p, p-q) that do not
 *          rise above the line y = x-r.
 * https://oeis.org/A050157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2r choose r) - (2r choose r+k+1) where r is the largest
 *            integer such that r(r+1)/2 ≤ n, and k = n – r(r+1)/2. This
 *            yields the entry T(r,k) of the OEIS triangle, returned in
 *            row‑major order
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, row_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/row_for_n.h"

seq_t a(idx_t n)
{
    /* Determine row and column in the triangle. */
    idx_t r = row_for_n(n);
    idx_t k = n - (unsigned __int128)r * (r + 1) / 2;

    /* Compute the binomial terms. */
    idx_t two_r = 2 * r;
    seq_t bin1 = binomial(two_r, r);
    seq_t bin2 = binomial(two_r, r + k + 1);

    return bin1 - bin2;
}
