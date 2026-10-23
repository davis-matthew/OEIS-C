/*
 * A019333  Expansion of g.f. 1/((1-4*x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A019333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^n - 9*6^n + 8*8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* 2 * 4^n */
    seq_t t4 = pow_seq((seq_t)4, n);
    t4 = mul_small(t4, 2);

    /* 9 * 6^n */
    seq_t t6 = pow_seq((seq_t)6, n);
    t6 = mul_small(t6, 9);

    /* 8 * 8^n */
    seq_t t8 = pow_seq((seq_t)8, n);
    t8 = mul_small(t8, 8);

    /* a(n) = (2*4^n) + (8*8^n) - (9*6^n) */
    seq_t r = t4 + t8;
    r -= t6;
    return r;
}
