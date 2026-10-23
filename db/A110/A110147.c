/*
 * A110147  a(n) = 10^((n^2-n)/2).
 * https://oeis.org/A110147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^((n^2-n)/2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_n.h"

seq_t a(idx_t n)
{
    /* exponent = n*(n-1)/2  (unsigned arithmetic so 0-1 = 0 for n=0) */
    idx_t exp = n * (n - 1) / 2;
    return pow_n((seq_t)10, exp);
}
