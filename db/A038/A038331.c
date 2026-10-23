/*
 * A038331  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*5^j.
 * https://oeis.org/A038331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 12^(i-j) * 5^j where i and j satisfy n =
 *            i*(i+1)/2 + j, with 0 ≤ j ≤ i. This enumerates the triangle
 *            entries (i,j) along anti‑diagonals
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 154, u128 n <= 536,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find i so that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    idx_t sum = 0;                /* entries before the current row */
    while (sum + i + 1 <= n) {
        sum += i + 1;
        ++i;
    }

    idx_t j = n - sum;            /* column index in row i */

    /* Compute binomial(i,j) and the powers of 12 and 5. */
    seq_t bin = binomial(i, j);
    seq_t pow12 = seq_pow(12, i - j);
    seq_t pow5  = seq_pow(5,  j);

    return bin * pow12 * pow5;
}
