/*
 * A138777  Triangle read by rows: T(n,k)=binomial(n-2k,3k+2) (n>=2,
 *          0<=k<=(n-2)/5).
 * https://oeis.org/A138777
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A138777 – triangle read by rows.
 * 
 * T(n,k) = binomial(n-2k, 3k+2)          (n ≥ 2, 0 ≤ k ≤ ⌊(n-2)/5⌋)
 * Row n contains ⌊(n+3)/5⌋ terms.
 * a(n) is the (n‑2)th element of the flattened triangle (offset 2).
 * 
 * To find a(n):
 *   1. Compute the zero‑based position p = n-2.
 *   2. Find the row r for which
 *          Σ_{m=2}^{r-1} ⌊(m+3)/5⌋ ≤ p < Σ_{m=2}^{r} ⌊(m+3)/5⌋
 *   3. Let k = p - Σ_{m=2}^{r-1} ⌊(m+3)/5⌋.
 *   4. Return T(r,k) = binomial(r-2k, 3k+2).
 * 
 * The implementation uses the provided binom() routine for the binomial
 * coefficient.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                      /* undefined below the offset */

    /* zero‑based position in the flattened triangle (offset 2) */
    idx_t pos = n - 2;

    idx_t r   = 2;                      /* current row number (n‑row) */
    idx_t cum = 0;                      /* cumulative count of terms seen */

    for (;;)
    {
        /* number of terms in row r: ⌊(r+3)/5⌋ */
        idx_t len = (r + 3) / 5;

        if (cum + len > pos)           /* the desired term is in this row */
        {
            idx_t k = pos - cum;       /* column index within the row     */
            return binom(r - 2 * k, 3 * k + 2);   /* T(r,k) */
        }

        cum += len;                     /* skip entire row */
        ++r;                             /* move to next row */
    }
}
