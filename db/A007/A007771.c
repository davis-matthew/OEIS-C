/*
 * A007771  Values of Ehrhart polynomial of dilation by 2 of Relaxed Boolean
 *          Quadric Polytope of order 4.
 * https://oeis.org/A007771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..9} h(k)*binomial(n+10-k, 10), with Ehrhart
 *            h*-vector h = [1, 213, 8064, 70712, 197490, 197490, 70712, 8064,
 *            213, 1]
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static const unsigned long hstar[10] = {
    1UL, 213UL, 8064UL, 70712UL, 197490UL,
    197490UL, 70712UL, 8064UL, 213UL, 1UL
};

seq_t a(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    for (k = 0; k < 10; k++)
        s += binom(n + 10 - k, 10) * (seq_t)hstar[k];
    return s;
}
