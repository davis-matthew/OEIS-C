/*
 * A128975  a(n) = the number of unordered triples of integers (a,b,c) with
 *          a+b+c=n, whose bitwise XOR is zero. Equivalently, the number of
 *          three-heap nim games with n stones which are in a losing position
 *          for the first player.
 * https://oeis.org/A128975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n is odd; otherwise a(n) = (3^(popcount(n)-1)-1)/2
 *            where popcount(n) is the number of 1's in the binary expansion
 *            of n
 * Range:     u8 n <= 253, u16 n <= 8189, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For odd n the result is zero. */
    if ((n & 1) != 0)
        return (seq_t)0;

    /* Count the number of 1-bits in n (popcount). */
    idx_t r = 0;
    for (idx_t x = n; x != 0; x >>= 1)
        r += x & 1U;   /* x & 1U extracts the least significant bit */

    /* Compute 3^(r-1); for r == 0 or 1 the exponent is 0 and the result is 1. */
    idx_t exp = (r > 0) ? r - 1 : 0;
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        pow *= (seq_t)3;

    /* Return (pow - 1) / 2.  All operations are integer and safe for seq_t. */
    return (pow - (seq_t)1) / (seq_t)2;
}
