/*
 * A176632  a(n) = 6*a(n-1)-8*a(n-2)-9 for n > 2; a(0) = 77, a(1) = 897, a(2)
 *          = 3333.
 * https://oeis.org/A176632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(64*4^n + 22*2^n - 1) for n>0, a(0)=77
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   mul_small, pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)77;

    /* 64 * 4^n */
    seq_t term = mul_small(pow4(n), (unsigned)64);

    /* + 22 * 2^n */
    term = term + mul_small(pow2(n), (unsigned)22);

    /* - 1 */
    term = term - (seq_t)1;

    /* * 3 */
    term = mul_small(term, (unsigned)3);

    return term;
}
