/*
 * A146880  A symmetrical triangle sequence of coefficients : p(x,n)=If[n ==
 *          0, 1, (x + 1)^n + Sum[(1 + Mod[Binomial[n, m], 2])*x^m*(1 + x^(n -
 *          2*m)), {m, 1, n - 1}]].
 * https://oeis.org/A146880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient a_{r,m} in the polynomial p(x,r) =
 *            (x+1)^r + Σ_{m=1}^{r-1} (1 + Mod[Binomial[r,m],2]) x^m (1 +
 *            x^{r-2m}), where (r,m) are the row and position determined by r
 *            = ⌊(√(8n+1)-1)/2⌋, m = n - r(r+1)/2
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Locate the row r that contains the index n.
       Row r has length r+1 and the cumulative size up to\n
       row r is (r+1)(r+2)/2.  The following loop\n
       finds the largest r such that (r+1)(r+2)/2 <= n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        r++;
    }

    /* Position within the row: 0 ≤ pos ≤ r. */
    idx_t pos = n - (r * (r + 1)) / 2;

    if (r == 0) {           /* only element of row 0 */
        return (seq_t)1;
    }

    /* Base contribution from (x+1)^r. */
    seq_t result = binom(r, pos);

    /* Add contributions from the sum in the definition.
       For each m in 1..r-1 add (1+parity) whenever
       pos==m or pos==r-m.  If pos==m==r-m the term
       is added twice. */
    for (idx_t m = 1; m < r; ++m) {
        int parity = ((m & (r - m)) == 0) ? 1 : 0;   /* (binomial r,m) mod 2 */
        seq_t add = (seq_t)(1 + parity);

        if (pos == m) {
            result += add;
        }
        if (pos == r - m) {
            result += add;
        }
    }

    return result;
}
