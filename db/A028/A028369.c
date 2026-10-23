/*
 * A028369  a(n) = 2^(2^(n-1) + n - 2).
 * https://oeis.org/A028369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2^(n-1) + n - 2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1u) {
        /* 2^(2^0 + 1 - 2) = 2^0 = 1 */
        return (seq_t)1;
    }

    /* Compute the exponent: 2^(n-1) + n - 2 */
    seq_t exp = pow2(n - 1u) + (seq_t)(n - 2u);

    /* a(n) = 2^exp */
    return big_pow((seq_t)2, exp);
}
