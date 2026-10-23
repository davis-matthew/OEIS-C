/*
 * A021336  Decimal expansion of 1/332.
 * https://oeis.org/A021336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/332, offset 0. The
 *            nth digit (a(0) = first digit after the decimal point) is found
 *            by repeated modular multiplication: r_0 = 1 r_{k+1} = (r_k * 10)
 *            mod 332 a(k) = floor((r_k * 10) / 332) The implementation
 *            follows this formula literally and runs in O(n...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

seq_t a(idx_t n)
{
    /* r holds 10^n modulo 332 */
    uint64_t r = 1;
    for (idx_t i = 0; i < n; ++i)
        r = (r * 10ULL) % 332ULL;

    /* compute the digit */
    uint64_t digit = (r * 10ULL) / 332ULL;
    return (seq_t)digit;
}
