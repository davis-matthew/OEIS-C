/*
 * A061441  Denominators of coefficients of even powers of x of the expansion
 *          of Bessel function J_10(x).
 * https://oeis.org/A061441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2n+10) * n! * (n+10)!
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t fac_n = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fac_n *= (seq_t)i;
    }

    seq_t fac_np10 = fac_n;
    for (idx_t i = n + 1; i <= n + 10; ++i) {
        fac_np10 *= (seq_t)i;
    }

    seq_t power = pow2((idx_t)(2 * n + 10));

    seq_t result = fac_n;
    result *= fac_np10;
    result *= power;
    return result;
}
