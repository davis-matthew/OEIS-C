/*
 * A036136  a(n) = 3^n mod 89.
 * https://oeis.org/A036136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 89
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod_89
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_mod_89.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (n > 0) {
        if (n & 1)
            result = mul_mod_89(result, base);
        base = mul_mod_89(base, base);
        n >>= 1;
    }
    return result;
}
