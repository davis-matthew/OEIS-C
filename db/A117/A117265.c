/*
 * A117265  Triangle T, read by rows, where matrix power T^-2 has -2^(n+1) in
 *          the secondary diagonal: [T^-2](n+1,n) = -2^(n+1), with all 1's in
 *          the main diagonal and zeros elsewhere.
 * https://oeis.org/A117265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 27, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A117265 (Triangle T read by rows).
 *
 * Formula (from the OEIS entry):
 *   T(n,k) = 2^( n*(n-1)/2 - k*(k-1)/2 ) *
 *            ∏_{j=0}^{n-k-1} (2*j+1) / (n-k)!,
 *   with T(n,n)=1.
 *   
 * a(n) is the nth term in the row‑major enumeration of T.
 * For a given n, let r be the row index and k the column index
 * (0‑based). Then a(n)=T(r,k).
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2) / 2) <= n ) {
        ++r;
    }
    /* Column index within the row */
    idx_t k = n - (r * (r + 1) / 2);
    /* Difference m = n - k */
    idx_t m = r - k;

    /* Exponent in the power of 2:  n*(n-1)/2 - k*(k-1)/2 */
    idx_t exp = (r * (r - 1) / 2) - (k * (k - 1) / 2);

    /* Start with the power of 2 term */
    seq_t val = pow2(exp);

    /* Multiply by the odd product ∏_{j=0}^{m-1} (2*j+1) */
    for (idx_t j = 0; j < m; ++j) {
        val *= (seq_t)(2 * j + 1);
    }

    /* Divide by the factorial m! */
    for (idx_t j = 2; j <= m; ++j) {
        val /= (seq_t)j;
    }

    return val;
}
