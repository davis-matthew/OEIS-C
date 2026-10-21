/*
 * A207536  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A105070; see Formula section.
 * https://oeis.org/A207536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened triangle of T(n,k)=binomial(n,2*k)*2^k,
 *            with n>=1
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but guard anyway */

    idx_t cum = 0;          /* cumulative count of terms up to previous row   */
    idx_t row = 0;          /* current row number (1‑based)                    */

    for (;;) {
        ++row;                            /* move to next row                         */
        idx_t len = (row / 2) + 1;        /* number of entries in this row            */

        if (cum + len >= n) {            /* the desired term lies in this row       */
            idx_t k = n - cum - 1;        /* column index within the row (0‑based)   */
            idx_t twok = k * 2;

            seq_t bin = binomial(row, twok);   /* binomial(row, 2k)                    */
            seq_t pow = pow2(k);               /* 2^k                                 */
            return bin * pow;                  /* T(row,k) = binomial * 2^k           */
        }

        cum += len;   /* advance cumulative counter to this row   */
    }
}
