/*
 * A174882  A (3/2,-1) Somos-4 sequence.
 * https://oeis.org/A174882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+4) = -16 · a(n) * Initial values: a(0)=1, a(1)=1, a(2)=-2,
 *            a(3)=-8
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 30, i64 n <= 62, i128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Initial terms for n mod 4 */
    static const seq_t init[4] = {1, 1, -2, -8};

    idx_t k = n >> 2;          /* floor(n/4) */
    idx_t m = n & 3;           /* n mod 4 */

    seq_t base = init[(int)m];

    /* Power of 2: 16^k = 2^(4k) */
    idx_t exp = k << 2;        /* 4*k */
    seq_t mult = pow2(exp);

    /* Apply the sign (-1)^k */
    if (k & 1)
        mult = -mult;

    return base * mult;
}
