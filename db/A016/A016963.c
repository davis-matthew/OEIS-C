/*
 * A016963  a(n) = (6*n + 4)^7.
 * https://oeis.org/A016963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 93, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t x = n;        /* cast n to seq_t */
    x = x * 6 + 4;      /* (6*n + 4) */
    return big_pow(x, 7);  /* raise to the 7th power */
}
