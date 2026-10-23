/*
 * A135309  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n having k UUUU's starting at level 0.
 * https://oeis.org/A135309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th element of the triangle read by rows. * Let
 *            T(n,k) be the number of Dyck paths of semilength n having * k
 *            occurrences of UUUU starting at level 0. * T(n,k) = Σ_{j=k}^{
 *            ⌊n/4⌋} (-1)^{j-k}·(5j+1)·C(j,k)·C(2n-3j, n+j)/(n+j+1), * where
 *            C(a,b) is the binomial coefficient. * a(n) is obt...
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Convert flat index to (row, k). */
    idx_t row = 0;
    idx_t remaining = n;
    while (true) {
        idx_t row_len = 1 + row / 4;    /* number of terms in this row   */
        if (remaining < row_len) {
            break;                     /* found the desired row         */
        }
        remaining -= row_len;
        ++row;
    }
    idx_t k = remaining;                 /* position inside the row       */

    /* Compute T(row,k) by direct summation. */
    idx_t maxj = row / 4;
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;

    for (idx_t j = k; j <= maxj; ++j) {
        /* binomial(j,k) and binomial(2*row-3*j, row+j) */
        seq_t bin1 = binomial(j, k);
        seq_t bin2 = binomial(2 * row - 3 * j, row + j);

        /* term = bin1 * bin2 * (5*j+1) / (row + j + 1) */
        seq_t term = bin1 * bin2;
        term = term * (seq_t)(5 * j + 1);
        term = term / (seq_t)(row + j + 1);

        if ((j - k) & 1U) {
            sum_odd += term;   /* j-k is odd   */
        } else {
            sum_even += term;  /* j-k is even  */
        }
    }

    /* The result is the difference; it must be non‑negative. */
    seq_t result = (sum_even < sum_odd) ? 0 : sum_even - sum_odd;
    return result;
}
