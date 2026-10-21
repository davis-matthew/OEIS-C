/*
 * A208657  Triangular array read by rows:
 *          n*binomial(n,n-k+1)-binomial(n-1,n-k) with k = 1..n, n >= 1.
 * https://oeis.org/A208657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = n*binomial(n,n-k+1)-binomial(n-1,n-k) where k = position
 *            of n within its row (k = 1..row), rows read left-to-right and
 *            the sequence is formed by concatenating the rows of a triangular
 *            array
 * Range:     u8 n <= 31, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Returns the n-th term of OEIS A208657 (1-indexed). */
seq_t a(idx_t n)
{
    /* Find the row r such that T(r-1) < n <= T(r), where T(m)=m*(m+1)/2. */
    idx_t r = 1;          /* current row */
    idx_t cum = 1;        /* cumulative count up to current row */

    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Position k within the row: k = n - T(r-1) */
    idx_t k = n - (cum - r);

    /* Compute the term using the provided binomial helper. */
    seq_t row = (seq_t)r;
    seq_t bin1 = binomial(r, r - k + 1);
    seq_t bin2 = binomial(r - 1, r - k);

    return row * bin1 - bin2;
}
