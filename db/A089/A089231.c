/*
 * A089231  Triangular array A066667 or A008297 unsigned and transposed.
 * https://oeis.org/A089231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 >= n, k =
 *            n - r(r-1)/2. T(r,k) = binomial(r, r-k+1) × ∏_{i=r-k+1}^{r-1} i
 *            (OEIS A089231)
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 75, u64 n <= 204,
 *            u128 n <= 553, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find the row r (1‑based) that contains the n‑th term */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) {
        ++r;
        sum += r;               /* sum of first r integers = r(r+1)/2 */
    }
    /* column index k (1‑based) within that row */
    idx_t k = n - (sum - r);

    /* r1 = r-k+1, appears many times below */
    idx_t r1 = r - k + 1;

    /* compute binomial coefficient C(r, r1) multiplicatively */
    seq_t binom = 1;
    for (idx_t i = 1; i <= r1; ++i) {
        binom *= (seq_t)(r - r1 + i);
        binom /= (seq_t)i;
    }

    /* multiply by the product ∏_{i=r1}^{r-1} i  (empty for k=1) */
    seq_t result = binom;
    for (idx_t i = r1; i <= r - 1; ++i) {
        result *= (seq_t)i;
    }

    return result;
}
