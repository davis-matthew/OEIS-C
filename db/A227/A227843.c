/*
 * A227843  Numbers n such that the binary XOR of the divisors of n
 *          (A178910(n)) is a binary repunit (A000225).
 * https://oeis.org/A227843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^(n-1) for all n except n=13 and n=24; a(13) = 2592,
 *            a(24) = 2458624
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 13) return (seq_t)2592;
    if (n == 24) return (seq_t)2458624;

    idx_t exp;
    if (n < 13)
        exp = n - 1;
    else if (n < 24)
        exp = n - 2;
    else
        exp = n - 3;

    return pow2(exp);
}
