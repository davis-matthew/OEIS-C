/*
 * A193899  Triangular array: the self-fusion of (p(n,x)), where
 *          p(n,x)=x*p(n-1,x)+2^n, p(0,x)=1.
 * https://oeis.org/A193899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * Let r and k be the row and column such that * n = r*(r+1)/2 +
 *            k (0 ≤ k ≤ r). * Then: * a(0) = 1 * for r > 0 * if k < r a(n) =
 *            2^(r-1-k) * ((4^(k+1)-1)/3) * if k = r a(n) = 2 * ((4^r-1)/3) *
 *            * The implementation uses the helper routine * seq_t pow2(idx_t
 *            exp) (defined in utils/pow2.h) for powers...
 * Range:     u8 n <= 18, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that T_r <= n < T_{r+1}, where T_r = r*(r+1)/2. */
    idx_t r = 0;
    while ( (r * (r + 1)) / 2 <= n ) ++r;
    --r;                                   /* now T_r <= n < T_{r+1} */
    /* Compute column index k in row r. */
    idx_t k = n - (r * (r + 1)) / 2;

    if (r == 0)        /* row 0 has the single entry 1 */
        return (seq_t)1;

    if (k == r) {
        /* a(n) = 2 * ((4^r - 1) / 3) */
        seq_t pow4 = pow2(2 * r);         /* 4^r = 2^(2r) */
        seq_t t    = pow4 - 1;
        t /= 3;
        return t * 2;
    } else {
        /* a(n) = 2^(r-1-k) * ((4^(k+1) - 1) / 3) */
        seq_t pow2_part = pow2(r - 1 - k);          /* 2^(r-1-k) */
        seq_t pow4      = pow2(2 * (k + 1));        /* 4^(k+1) = 2^(2(k+1)) */
        seq_t t          = pow4 - 1;
        t /= 3;
        return pow2_part * t;
    }
}
