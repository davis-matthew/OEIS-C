/*
 * A176567  Triangle T(n, k) = binomial(binomial(n, 2) + k, k) +
 *          binomial(binomial(n, 2) + n-k, n-k) - binomial(binomial(n, 2) + n,
 *          n), read by rows.
 * https://oeis.org/A176567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n, k) = binomial(binomial(n, 2) + k, k) + binomial(binomial(n,
 *            2) + n–k, n–k) – binomial(binomial(n, 2) + n, n); a(n) =
 *            sum_{k=0}^{n} T(n, k)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 55, i64 n <= 136,
 *            i128 n <= 326, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row index r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n) {
        ++r;
    }
    --r;

    /* Position of n within its row: k = n - start_of_row */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* binomial(n, 2) computed as r*(r-1)/2 */
    idx_t m = r * (r - 1) / 2;

    seq_t term1 = binomial(m + k, k);
    seq_t term2 = binomial(m + (r - k), r - k);
    seq_t term3 = binomial(m + r, r);

    return term1 + term2 - term3;
}
