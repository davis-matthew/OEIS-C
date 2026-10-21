/*
 * A086577  a(n) = 6*(10^n - 1).
 * https://oeis.org/A086577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t ten = pow10(n);               /* 10^n  */
    return (ten - 1) * 6;               /* 6*(10^n - 1) */
}
