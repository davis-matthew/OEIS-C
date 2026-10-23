/*
 * A135330  Triangle read by rows: T(n,k) is the number of Dyck paths of
 *          semilength n having k UUDU's starting at level 0.
 * https://oeis.org/A135330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = value of the triangle T(row,k) for the index n. T(n,k) is
 *            given by: T(n,k) = 1/(n+1) * Σ_{j=k}^{⌊n/3⌋} (-1)^{j-k} (3j+1) *
 *            C(j,k) * C(2n-3j, n)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row and column of the n-th term in the row-major triangle. */
    idx_t sum = 0;
    idx_t row = 0;
    while (true) {
        idx_t len = 1 + (row / 3);        /* number of terms in this row */
        if (sum + len > n)               /* n is inside this row */
            break;
        sum += len;
        row++;
    }
    idx_t k = n - sum;                    /* column index within the row */

    /* Compute T(row,k) using the closed formula. */
    const idx_t max_j = row / 3;
    seq_t pos = 0, neg = 0;               /* accumulate positive and negative terms separately */

    for (idx_t j = k; j <= max_j; ++j) {
        seq_t term = binomial(j, k);                       /* C(j,k) */
        term *= binomial(2 * row - 3 * j, row);            /* C(2n-3j, n) */
        term *= (seq_t)(3 * j + 1);                        /* (3j+1) */
        if ((j - k) & 1) {
            neg += term;                                   /* subtract if sign is negative */
        } else {
            pos += term;                                   /* add if sign is positive */
        }
    }

    seq_t diff = pos - neg;                                /* Σ (-1)^{j-k} (3j+1) C(j,k) C(2n-3j,n)  */
    seq_t result = diff / (seq_t)(row + 1);                 /* divide by n+1 */
    return result;
}
