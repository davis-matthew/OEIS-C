/*
 * A173034  Expansion of g.f. 8/(1-2*z)-12/(1-z)+z+5.
 * https://oeis.org/A173034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=5, a(n)=8*2^n-12 for n>=2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    return (seq_t)8 * pow2(n) - (seq_t)12;
}
