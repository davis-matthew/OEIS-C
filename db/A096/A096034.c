/*
 * A096034  Triangle read by rows: T(n,k) = (n+1,k)-th element of (M^3-M)/2,
 *          where M is the infinite lower Pascal's triangle matrix, 1<=k<=n.
 * https://oeis.org/A096034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle read by rows * with
 *            T(row,col) = binomial(row, col-1) * (3^(row - col + 1) - 1) / 2
 *            * where row and col are obtained by unfolding the triangular
 *            sequence: * row = smallest r such that r*(r+1)/2 ≥ n * col = n -
 *            r*(r-1)/2 (1‑based column)
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find the row containing the n-th term */
    idx_t row     = 1;
    idx_t cumulative = 1;          /* elements up to current row */
    while (cumulative < n) {
        row++;
        cumulative += row;
    }

    /* Elements before this row */
    idx_t prev_cumulative = cumulative - row;
    idx_t col = n - prev_cumulative;          /* 1‑based column index */

    /* Compute the term */
    idx_t k_minus1 = col - 1;
    seq_t bin      = binomial((idx_t)row, k_minus1);

    /* 3^(row - k + 1) = 3^(row - k_minus1) */
    seq_t pow3 = pow_seq((seq_t)3, (int)(row - k_minus1));
    seq_t diff = pow3 - (seq_t)1;            /* 3^d - 1  (always even) */

    seq_t result = (bin * diff) / (seq_t)2;  /* exact integer division */
    return result;
}
