/*
 * A050163  T(n, k) = S(2n+2, n+2, k+2) for 0<=k<=n and n >= 0, array S as in
 *          A050157.
 * https://oeis.org/A050163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(2r+2, r) - binomial(2r+2, r+c+3), where r and c
 *            are the row and column indices such that n = r(r+1)/2 + c (0 ≤ c
 *            ≤ r). This follows from T(n,k) = binomial(2n+2,n) −
 *            binomial(2n+2,n+k+3) in the OEIS entry
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* Provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column c within the row.  */
    idx_t c = n - r * (r + 1) / 2;

    /* Compute the two binomial terms.  */
    idx_t N   = 2 * r + 2;       /* upper index */
    seq_t bin1 = binomial(N, r); /* binomial(2r+2, r)   */

    idx_t k2 = r + c + 3;        /* lower index for second binomial */
    seq_t bin2;
    if (k2 > N) {
        bin2 = 0;
    } else {
        bin2 = binomial(N, k2);
    }

    return bin1 - bin2;
}
