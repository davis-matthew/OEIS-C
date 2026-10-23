/*
 * A013716  a(n) = 11^(2*n + 1).
 * https://oeis.org/A013716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^(2*n + 1)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute exponent: 2*n + 1 */
    seq_t exp = 2 * (seq_t)n + 1;
    /* 11 raised to the computed exponent */
    return big_pow((seq_t)11, exp);
}
