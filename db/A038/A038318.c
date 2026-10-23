/*
 * A038318  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*4^j.
 * https://oeis.org/A038318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 11^(i-j) * 4^j where n = i*(i+1)/2 + j
 *            and 0 ≤ j ≤ i
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t k = 0; k < exp; ++k)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* Determine the row i such that i*(i+1)/2 ≤ n < (i+1)*(i+2)/2. */
    idx_t i = 0;
    idx_t tri = 0;
    while (tri <= n) {
        ++i;
        tri = i * (i + 1) / 2;
    }
    --i;                       /* now i satisfies the inequality */
    tri = i * (i + 1) / 2;
    idx_t j = n - tri;         /* 0 ≤ j ≤ i */

    seq_t binom = binomial(i, j);

    seq_t pow11 = pow_seq((seq_t)11, i - j);
    seq_t pow4  = pow_seq((seq_t)4,  j);

    return binom * pow11 * pow4;
}
