/*
 * A218034  Number of ways to seat 4 types of people in n labeled seats around
 *          a circle such that no two adjacent people are of the same type.
 * https://oeis.org/A218034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(n)=3^n + 3*(-1)^n for n>=2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    if ((n & 1) == 0)    /* even n: add 3 */
        return pow3 + (seq_t)3;
    else                 /* odd n: subtract 3 */
        return pow3 - (seq_t)3;
}
