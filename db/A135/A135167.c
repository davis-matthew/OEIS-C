/*
 * A135167  a(n) = 7^n + 5^n + 3^n - 2^n. Constants are the prime numbers in
 *          decreasing order.
 * https://oeis.org/A135167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 7^n + 5^n + 3^n - 2^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t v7 = pow_uint(7, n);
    seq_t v5 = pow_uint(5, n);
    seq_t v3 = pow_uint(3, n);
    seq_t v2 = pow_uint(2, n);
    return v7 + v5 + v3 - v2;
}
