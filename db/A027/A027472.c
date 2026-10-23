/*
 * A027472  Third convolution of the powers of 3 (A000244).
 * https://oeis.org/A027472
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-3) * binomial(n-1, 2) = 3^(n-3) * (n-1)*(n-2)/2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 37, u128 n <= 76,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"   /* provides seq_t pow3(idx_t exp) */

seq_t a(idx_t n)
{
    if (n < 3) return 0;   /* offset is 3; behaviour below 3 does not matter */

    /* binomial(n-1, 2) = (n-1)*(n-2)/2 */
    seq_t binom = ((seq_t)(n-1) * (seq_t)(n-2)) / 2;

    /* 3^(n-3) */
    seq_t power = pow3(n-3);

    return binom * power;
}
