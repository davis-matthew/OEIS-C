/*
 * A038250  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*8^j.
 * https://oeis.org/A038250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 5^(i-j) * 8^j * where i and j satisfy n =
 *            i*(i+1)/2 + j, 0 <= j <= i * (flattening the triangle row by
 *            row, left‑to‑right)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mul_pow_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_pow_small.h"

seq_t a(idx_t n)
{
    /* Find the row index i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 <= n)
        ++i;

    /* Compute j = n - i*(i+1)/2. */
    idx_t j = n - (i * (i + 1)) / 2;

    /* Compute binomial(i, j) multiplicatively. */
    seq_t bin = 1;
    for (idx_t k = 1; k <= j; ++k) {
        bin = bin * (i - j + k);
        bin = bin / k;                     /* exact division */
    }

    /* Compute 5^(i-j) and 8^j. */
    seq_t pow5 = mul_pow_small(5, i - j);
    seq_t pow8 = mul_pow_small(8, j);

    return bin * pow5 * pow8;
}
