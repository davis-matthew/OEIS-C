/*
 * A038265  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*11^j.
 * https://oeis.org/A038265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 6^(i-j) * 11^j, where i and j are
 *            obtained from the triangular indexing: start_i = i*(i+1)/2
 *            satisfies start_i <= n < (i+1)*(i+2)/2, and j = n - start_i
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row i such that start_i <= n. */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 <= n)
        ++i;

    /* Compute j = position within the row. */
    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;

    /* Compute binomial coefficient. */
    seq_t val = binomial(i, j);

    /* Multiply by 6^(i-j). */
    for (idx_t k = 0; k < i - j; ++k)
        val *= (seq_t)6;

    /* Multiply by 11^j. */
    for (idx_t k = 0; k < j; ++k)
        val *= (seq_t)11;

    return val;
}
