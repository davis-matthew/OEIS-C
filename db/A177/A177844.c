/*
 * A177844  a(n) = 6*a(n-1)-8*a(n-2) for n > 3; a(0)=279, a(1)=3996,
 *          a(2)=16008, a(3)=64784.
 * https://oeis.org/A177844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n+5)-47*2^(n+1) for n>1; a(0)=279, a(1)=3996
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 59,
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
    if (n == 0) return (seq_t)279;
    if (n == 1) return (seq_t)3996;

    /* For n >= 2 use the closed form */
    seq_t term1 = pow4((idx_t)(n + 5));               /* 4^(n+5) */
    seq_t term2 = mul_small(pow2((idx_t)(n + 1)), 47);/* 47*2^(n+1) */
    return term1 - term2;
}
