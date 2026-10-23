/*
 * A017196  a(n) = (9*n + 2)^12.
 * https://oeis.org/A017196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 2)^12
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 4, u128 n <= 180,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute base = 9*n + 2 */
    seq_t base = 9 * (seq_t)n + 2;
    /* Raise to the 12th power */
    return big_pow(base, (seq_t)12);
}
