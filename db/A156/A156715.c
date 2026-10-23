/*
 * A156715  Let k=1; then T(n,m) = (((2*k+1)/(m+k+1))*binomial(n-1-k,
 *          m-k)*binomial(n+k, m+k) + ((2*k+1)/(n-m+k))*binomial(n-1-k,
 *          n-m-1-k)*binomial(n+k, n-m-1+k)), irregular triangle.
 * https://oeis.org/A156715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, less
 * Formula:   a(n) is the flattened sequence of the irregular triangle T(n,m)
 *            = ((2k+1)/(m+k+1))*C(n-1-k,m-k)*C(n+k,m+k)
 *            +((2k+1)/(n-m+k))*C(n-1-k,n-m-1-k)*C(n+k,n-m-1+k) with k = 1.
 *            The triangle rows have lengths r+2; a(n) is T(r,m) with r the
 *            row containing position n and m the column (0‑based)
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative length up to r exceeds n.
       For row r the number of elements is r+2, so
       cumulative(r) = Σ_{i=0}^{r} (i+2) = (r+1)(r+4)/2.          */
    idx_t r = 0;
    while (( (r + 1) * (r + 4) / 2 ) <= n)
        ++r;

    /* Starting index of this row.                      */
    idx_t start = r * (r + 3) / 2;

    /* Column within the row.                            */
    idx_t m = n - start;                /* 0 ≤ m ≤ r+1 */

    /* Parameters used in the formula.                  */
    seq_t term1, term2;
    idx_t n_par = r + 2;                /* n used in T(n,m) */

    /* --- Compute first term -------------------------------------- */
    /* 3/(m+2) * C(n-2, m-1) * C(n+1, m+1)                  */
    seq_t bin1 = 0;
    if (m > 0)                       /* m-1 is valid only if m>0   */
        bin1 = binomial(n_par - 2, m - 1);

    /* Note: C(n+1, m+1) is always defined because m ≤ r+1 ≤ n_par+1. */
    seq_t bin2 = binomial(n_par + 1, m + 1);

    term1 = ((seq_t)3 * bin1 * bin2) / (m + 2);

    /* --- Compute second term ------------------------------------ */
    /* 3/(n-m+1) * C(n-2, n-m-2) * C(n+1, n-m)              */
    seq_t bin3 = 0;
    if (m <= n_par - 2)               /* n-m-2 is valid only if m ≤ n_par-2 */
        bin3 = binomial(n_par - 2, n_par - m - 2);

    seq_t bin4 = binomial(n_par + 1, n_par - m);

    term2 = ((seq_t)3 * bin3 * bin4) / (n_par - m + 1);

    return term1 + term2;
}
