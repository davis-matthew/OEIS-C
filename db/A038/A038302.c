/*
 * A038302  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*12^j.
 * https://oeis.org/A038302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 9^(i-j) * 12^j, where i = ⌊(√(8n+1)-1)/2⌋
 *            and j = n - i*(i+1)/2 (triangular indexing)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find i such that i*(i+1)/2 <= n < (i+1)*(i+2)/2 */
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        ++i;
    }
    /* Compute j */
    idx_t tri = i * (i + 1) / 2;
    idx_t j = n - tri;

    /* Base binomial coefficient */
    seq_t val = binomial(i, j);

    /* Multiply by 9^(i-j) */
    idx_t e9 = i - j;
    for (idx_t k = 0; k < e9; ++k) {
        val = val * (seq_t)9;
    }

    /* Multiply by 12^j */
    for (idx_t k = 0; k < j; ++k) {
        val = val * (seq_t)12;
    }

    return val;
}
