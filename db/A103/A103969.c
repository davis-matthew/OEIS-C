/*
 * A103969  Positions n such that A005941(n) = A005940(n).
 * https://oeis.org/A103969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = n for n <= 4 a(n) = (5 + (n-5) % 5) * 2^((n-5)/5) for n
 *            >= 5
 * Range:     u8 n <= 32, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required helper to compute powers of two */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 4) {
        return (seq_t)n;
    }
    /* n >= 5 */
    idx_t t = n - 5;
    idx_t block = t / 5;          /* exponent of 2   */
    idx_t r = t % 5;              /* offset 0..4      */
    seq_t base = (seq_t)(5 + r);  /* 5,6,7,8,9        */
    seq_t two_pow = pow2(block);  /* 2^block          */
    return base * two_pow;
}
