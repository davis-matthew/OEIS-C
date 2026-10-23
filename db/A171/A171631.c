/*
 * A171631  Triangle read by rows: T(n,k) = n*(binomial(n-2, k-1) +
 *          n*binomial(n-2, k)), n > 0 and 0 <= k <= n - 1.
 * https://oeis.org/A171631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(m,k) where T(n,k) =
 *            n*(binomial(n-2,k-1)+n*binomial(n-2,k)) and m is the unique row
 *            such that m*(m+1)/2 ≥ n. k = n - (m-1)*m/2 - 1
 * Range:     u8 n <= 23, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* offset 1 – behaviour for n<1 irrelevant */
    if (n == 1) return (seq_t)1;/* first triangle element */

    /* find row m such that sum_{i=1}^{m} i ≥ n  (i.e. m*(m+1)/2 ≥ n) */
    idx_t m = 1;
    while (((unsigned __int128)m * (m + 1)) / 2 < n) ++m;

    /* number of elements before row m */
    idx_t base = ((unsigned __int128)(m - 1) * m) / 2;
    /* column index within the row (0‑based) */
    idx_t k = n - base - 1;

    seq_t row_seq = (seq_t)m;

    seq_t bin1 = 0, bin2 = 0;

    /* compute binomial components; guard against underflow and invalid ranges */
    if (k > 0 && k - 1 <= m - 2) {
        bin1 = binom(m - 2, k - 1); /* C(m-2, k-1) */
    }
    if (k <= m - 2) {
        bin2 = binom(m - 2, k);     /* C(m-2, k)   */
    }

    /* final triangle value: n*(bin1 + n*bin2) */
    return row_seq * (bin1 + row_seq * bin2);
}
