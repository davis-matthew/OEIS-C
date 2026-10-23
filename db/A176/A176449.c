/*
 * A176449  a(n) = 9*2^n - 2.
 * https://oeis.org/A176449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*2^n - 2
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n using the utility, then 9*2^n - 2 */
    return 9 * pow2(n) - 2;
}
