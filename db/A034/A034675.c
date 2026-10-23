/*
 * A034675  Sum of n-th powers of divisors of 144.
 * https://oeis.org/A034675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2^n + 2^{2n} + 2^{3n} + 2^{4n}) * (1 + 3^n + 3^{2n})
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For any n, compute the sums of powers of 2 and 3
       multiplied together.  No special cases are needed
       because 0^0=1 for each pow_uint invocation.        */
    seq_t sum2 = 0;
    for (idx_t j = 0; j <= 4; ++j)
        sum2 += pow_uint(2, j * n);

    seq_t sum3 = 0;
    for (idx_t j = 0; j <= 2; ++j)
        sum3 += pow_uint(3, j * n);

    return sum2 * sum3;
}
