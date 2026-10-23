/*
 * A038298  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*8^j.
 * https://oeis.org/A038298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i, j) * 9^(i-j) * 8^j, where i = ⌊(√(8n+1)-1)/2⌋
 *            and j = n - i(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, floor_sqrt, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute i = floor((sqrt(8*n+1)-1)/2) */
    seq_t temp = (seq_t)n * (seq_t)8 + (seq_t)1;
    seq_t sqrtv = floor_sqrt(temp);
    seq_t half_step = sqrtv - (seq_t)1;
    idx_t i = (idx_t)(half_step / (seq_t)2);

    /* compute start index of row i and j = offset in row */
    idx_t start = i * (i + 1) / 2;
    idx_t j = n - start;

    /* compute binomial coefficient and powers */
    seq_t bin = binomial((idx_t)i, j);
    seq_t p9 = pow_uint(9, i - j);
    seq_t p8 = pow_uint(8, j);

    return bin * p9 * p8;
}
