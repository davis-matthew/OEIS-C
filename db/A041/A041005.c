/*
 * A041005  Convolution of Catalan numbers A000108(n+1), n >= 0, with A020918.
 * https://oeis.org/A041005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+7,3)*binomial(2*(n+4),n+2)/20 -
 *            (n+4)*(n+3)*4^(n+1)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 8, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* first part: binomial(n+7,3) */
    seq_t c1 = binomial(n + 7, 3);

    /* second part: binomial(2*(n+4), n+2) = binomial(2*n+8, n+2) */
    seq_t c2 = binomial(2 * n + 8, n + 2);

    /* product and division by 20 */
    seq_t part1 = (c1 * c2) / 20;

    /* 4^(n+1) */
    seq_t pow4 = pow_seq((seq_t)4, (int)(n + 1));

    /* (n+4)*(n+3)*4^(n+1) */
    seq_t part2 = (seq_t)(n + 4) * (seq_t)(n + 3) * pow4;

    /* final result */
    return part1 - part2;
}
