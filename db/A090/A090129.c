/*
 * A090129  Smallest exponent such that -1 + 3^a(n) is divisible by 2^n.
 * https://oeis.org/A090129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=2, a(n)=2^(n-2) for n≥3
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    /* For n ≥ 3: the order of 3 modulo 2^n is 2^(n-2) */
    return (seq_t)1 << (size_t)(n - 2);
}
