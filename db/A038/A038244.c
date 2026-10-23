/*
 * A038244  Triangle whose (i,j)-th entry is binomial(i,j)*5^(i-j)*2^j.
 * https://oeis.org/A038244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 5^(i-j) * 2^j where i is the unique
 *            integer with i*(i+1)/2 <= n < (i+1)*(i+2)/2 and j = n -
 *            i*(i+1)/2
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial, pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/power5.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row index i (small enough for idx_t multiplication). */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n)
        ++i;

    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;

    /* Compute the triangular entry. */
    seq_t result = binomial(i, j);
    result *= power5(i - j);
    result *= pow2(j);
    return result;
}
