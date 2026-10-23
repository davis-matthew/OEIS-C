/*
 * A013784  a(n) = 6^(4*n + 1).
 * https://oeis.org/A013784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(4*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent 4*n + 1 in the seq_t type */
    seq_t exp = (seq_t)(4 * n + 1);
    /* Return 6 raised to that exponent using the provided big_pow helper */
    return big_pow(6, exp);
}
