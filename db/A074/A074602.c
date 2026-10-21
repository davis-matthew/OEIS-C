/*
 * A074602  a(n) = 2^n + 7^n.
 * https://oeis.org/A074602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 7^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_u.h"

seq_t a(idx_t n)
{
    return pow_u((seq_t)2, n) + pow_u((seq_t)7, n);
}
