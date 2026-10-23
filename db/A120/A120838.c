/*
 * A120838  a(n) = 5^(n^2)+3^(n^2).
 * https://oeis.org/A120838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n^2)+3^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* exponent n^2 */
    idx_t exp = n * n;

    /* compute 5^exp and 3^exp using the provided big–power routine */
    seq_t pow5 = pow_uint(5, exp);
    seq_t pow3 = pow_uint(3, exp);

    /* sum the two terms */
    return pow5 + pow3;
}
