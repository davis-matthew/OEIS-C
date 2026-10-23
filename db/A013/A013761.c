/*
 * A013761  a(n) = 17^(3*n + 2).
 * https://oeis.org/A013761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17^(3*n + 2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* compute exponent 3*n + 2 */
    idx_t exp = n * 3 + 2;
    /* return 17 raised to that exponent */
    return seq_pow((seq_t)17, exp);
}
