/*
 * A147589  Concatenation of 2n-1 digits 1 and n-1 digits 0.
 * https://oeis.org/A147589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = ((10^(2*n-1) - 1)*10^(n-1))/9
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* exponent for the leading block of 1's */
    idx_t exp_ones = 2*n - 1;
    /* exponent for the trailing block of 0's */
    idx_t exp_zeros = n - 1;

    /* compute 10^(2*n-1) and 10^(n-1) */
    seq_t pow_ones  = pow10(exp_ones);
    seq_t pow_zeros = pow10(exp_zeros);

    /* apply the explicit formula */
    return (pow_ones - 1) * pow_zeros / 9;
}
