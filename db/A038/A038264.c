/*
 * A038264  Triangle whose (i,j)-th entry is binomial(i,j)*6^(i-j)*10^j.
 * https://oeis.org/A038264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*6^(i-j)*10^j where n = i*(i+1)/2 + j, 0 ≤ j
 *            ≤ i. i is the row index, j is the column index in that row
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        result *= base;
        --exp;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Find the row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }

    /* Column j within that row */
    idx_t j = n - i * (i + 1) / 2;

    /* Compute binomial(i,j) */
    seq_t bin = binomial(i, j);

    /* Compute 6^(i-j) and 10^j */
    seq_t p6 = pow_seq((seq_t)6, i - j);
    seq_t p10 = pow_seq((seq_t)10, j);

    /* Multiply all parts */
    seq_t result = bin;
    result *= p6;
    result *= p10;

    return result;
}
