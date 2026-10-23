/*
 * A141597  Triangle T(n,k) = 2*binomial(n,k)^2 - 1, read by rows, 0<=k<=n.
 * https://oeis.org/A141597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n+1)-st element of the triangle T(r,k) = 2 *
 *            binomial(r,k)² – 1 read by rows
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 179, u64 n <= 644,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Convert global index n to row r and column k in the triangle. */
static void
index_to_row_col(idx_t n, idx_t *r, idx_t *k)
{
    idx_t row = 0;
    idx_t cumulative = 1;            /* number of terms up to current row */
    while (n >= cumulative) {
        row++;
        cumulative += row + 1;       /* next row has row+1 elements */
    }
    *r = row;
    *k = n - row * (row + 1) / 2;    /* offset inside the row */
}

seq_t a(idx_t n)
{
    idx_t r, k;
    index_to_row_col(n, &r, &k);

    seq_t c = binomial(r, k);
    seq_t res = (c * c) * 2 - 1;
    return res;
}
