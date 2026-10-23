/*
 * A105260  Triangle read by rows: T(n,k)=C(2n-2k,k), n>=0, 0<=k<=floor(2n/3).
 * https://oeis.org/A105260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binom(2*r-2*k, k), where r = the unique row such that
 *            sum_{i=0}^{r-1} (floor(2*i/3)+1) <= n < sum_{i=0}^{r}
 *            (floor(2*i/3)+1), and k = n - sum_{i=0}^{r-1} (floor(2*i/3)+1).
 *            The triangle entry T(r,k) is defined as C(2r-2k, k). Thus the
 *            sequence a(n) lists the triangle entries row‑by‑row
 * Range:     u8 n <= 26, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t r = 0;

    /* Find the row r that contains the linear index n */
    while (1)
    {
        idx_t len = (2 * r) / 3 + 1;   /* number of entries in row r   */
        if (n < len)
            break;
        n -= len;
        ++r;
    }

    idx_t k = n;              /* column index within row r   */
    idx_t N = 2 * r - 2 * k;   /* choose upper argument       */

    return binom(N, k);
}
