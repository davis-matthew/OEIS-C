/*
 * A038321  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*7^j.
 * https://oeis.org/A038321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 11^(i-j) * 7^j, where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2. The triangle is
 *            listed row by row: for i = 0,1,2,... and j = 0,...,i
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 45, u64 n <= 140, u128 n <= 535,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper functions for binomial coefficients and power computations */
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Determine row index i and cumulative sum up to previous row. */
    idx_t i = 0;
    idx_t sum = 0;
    while (sum <= n) {
        sum += i + 1; /* add length of current row: i+1 elements */
        ++i;
    }
    /* After the loop: i = first row index that would make cumulative > n.
       Hence the actual row index is i-1, and the cumulative sum up to
       the previous row is sum - i. */
    idx_t row = i - 1;
    idx_t sum_prev = sum - i;

    /* Position within the row. */
    idx_t j = n - sum_prev;

    /* Compute binomial coefficient and powers of 11 and 7. */
    seq_t bin   = binomial(row, j);
    seq_t pow11 = pow_uint(11, row - j);
    seq_t pow7  = pow_uint(7, j);

    /* Multiply them to obtain the term. */
    return bin * pow11 * pow7;
}
