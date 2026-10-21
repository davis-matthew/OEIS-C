/*
 * A083587  a(n) = 4*3^n/3 - 5*0^n/6 - (n-1)2^(n-1).
 * https://oeis.org/A083587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*3^(n-1) - (n-1)*2^(n-1) for n>=1, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   mul_small, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* 4 * 3^(n-1) */
    seq_t p3 = pow3(n - 1);
    seq_t term1 = mul_small(p3, 4);

    /* (n-1) * 2^(n-1) */
    seq_t p2 = pow2(n - 1);
    seq_t term2 = mul_small(p2, (unsigned int)(n - 1));

    return term1 - term2;
}
