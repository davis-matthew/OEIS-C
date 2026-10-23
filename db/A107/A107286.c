/*
 * A107286  a(0) = 0; for n>0, minimal prime factor of n, or 1 if n is 1 or a
 *          prime.
 * https://oeis.org/A107286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = minimal prime factor of n, or 1 if n is 1 or n is prime;
 *            a(0)=0
 * Range:     u8 n <= 7173, u16 n <= 7173, u32 n <= 7173, u64 n <= 7173,
 *            u128 n <= 7173, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return 0;
    if (n <= 1)
        return 1;   /* a(1) = 1 */

    /* 2 is prime – special case */
    if (n == 2)
        return 1;

    uint64_t nn = (uint64_t)n;

    /* Even numbers (other than 2) have smallest prime factor 2 */
    if ((nn & 1ULL) == 0ULL)
        return (seq_t)2;

    /* Trial division: test odd divisors up to sqrt(n) */
    for (uint64_t d = 3; d * d <= nn; d += 2) {
        if (nn % d == 0ULL)
            return (seq_t)d;
    }

    /* No divisor found → n is prime */
    return 1;
}
