/*
 * A053312  a(n) contains n digits (either '1' or '2') and is divisible by
 *          2^n.
 * https://oeis.org/A053312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n+1) = a(n) + 10ⁿ · (2 – (a(n)/2ⁿ mod 2)), a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t val = 2;                /* a(1)                     */
    if (n <= 1) return val;

    seq_t pow10 = 10;             /* 10ⁿ⁻¹ for current n       */
    seq_t pow2  = 2;              /* 2ⁿ⁻¹ for current n       */

    for (idx_t i = 2; i <= n; ++i) {
        /* Check if val is divisible by 2ⁱ : modulo 2ⁱ⁺¹. */
        seq_t twoP = pow2;
        twoP <<= 1;               /* now twoP = 2ⁱ             */

        bool div = (val % twoP) == 0;

        /* Add the next digit part. */
        seq_t add = pow10 * (div ? 2 : 1);
        val += add;

        /* Prepare for the next iteration. */
        pow10 *= 10;
        pow2 <<= 1;                /* next pow2 = 2ⁱ             */
    }

    return val;
}
