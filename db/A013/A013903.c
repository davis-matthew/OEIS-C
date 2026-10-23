/*
 * A013903  a(n) = 22^(5*n + 2).
 * https://oeis.org/A013903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22^(5*n+2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent = 5*n + 2 */
    seq_t exp = (seq_t)5 * n + (seq_t)2;
    return big_pow((seq_t)22, exp);
}
