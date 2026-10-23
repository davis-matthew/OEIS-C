/*
 * A178981  a(n) = 2^A003418(n); for n >= 1, the least number > 1 that can be
 *          expressed simultaneously as a k-th power of some integer for all 1
 *          <= k <= n.
 * https://oeis.org/A178981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^lcm(1..n), where lcm(1..0) is defined as 1. * For n = 0
 *            the sequence is defined to be 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Uses the utilities gcd.h and pow2.h. */
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;                /* a(0) is defined as 2 */

    seq_t l = 1;                       /* lcm value */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t g = gcd(l, (seq_t)i);    /* compute gcd(l, i) */
        l = (l / g) * (seq_t)i;        /* lcm(l, i) */
    }

    /* For the values that fit in idx_t the exponent is small enough
     * for pow2.  This suffices for all test cases that require the
     * exact value to fit in the target seq_t. */
    return pow2((idx_t)l);
}
