/*
 * A017602  a(n) = (12*n + 6)^10.
 * https://oeis.org/A017602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 6)^10
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 6, u128 n <= 593,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute the base (12*n + 6) in seq_t type */
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)6;
    /* Raise the base to the 10th power using the utility big_pow */
    return big_pow(base, (seq_t)10);
}
