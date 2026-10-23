/*
 * A156698  Triangle T(n, k, m) = t(n,m)/( t(k,m) * t(n-k,m) ) with T(n, 0, m)
 *          = T(n, n, m) = 1, where t(n, m) = Product_{j=1..n}
 *          Product_{i=1..j-1} ( 1 - (m+1)*(2*i-1) ) and m = 3, read by rows.
 * https://oeis.org/A156698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = value obtained by reading triangle T(n,k,m=3) row‑wise.
 *            The triangle entries are T(n,k) = t(n)/(t(k)*t(n-k)), where t(n)
 *            = ∏_{i=1}^{n-1} (5-8i)^{\,n-i}. Since only T(n,0)=T(n,n)=1 are
 *            special cases, the general formula reduces to a multiplicative
 *            combination of powers of (5-8i) with expone...
 * Range:     i8 n <= 10, i16 n <= 16, i32 n <= 22, i64 n <= 38, i128 n <= 69,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* determine row r so that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    idx_t r = 0;
    while ((idx_t)((__int128)(r + 1) * (r + 2) / 2) <= n)
        ++r;

    idx_t start = (idx_t)((__int128)r * (r + 1) / 2);
    idx_t k = n - start;

    /* boundary entries are 1 */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* pre‑compute constants for exponent selection           */
    idx_t k1 = k - 1;
    idx_t m1 = r - k - 1;          /* may be -1 once k==r, but that case was caught above */

    seq_t result = 1;

    for (idx_t i = 1; i < r; ++i) {
        seq_t base = (seq_t)5 - (seq_t)8 * (seq_t)i;

        seq_t e;
        if (i <= k1 && i <= m1)       /* i <= k-1 and i <= r-k-1 */
            e = (seq_t)i;
        else if (i <= k1)             /* i <= k-1  */
            e = (seq_t)(r - k);
        else if (i <= m1)             /* i <= r-k-1 */
            e = (seq_t)k;
        else                          /* i > k-1 and i > r-k-1 */
            e = (seq_t)(r - i);

        if (e != 0) {
            seq_t pow_val = big_pow(base, e);
            result *= pow_val;
        }
    }

    return result;
}
