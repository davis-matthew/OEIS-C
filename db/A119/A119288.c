/*
 * A119288  a(n) is the second smallest prime factor of n, or 1 if n is a
 *          prime power.
 * https://oeis.org/A119288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A119288:  a(n) = the second smallest prime factor of n,
    or 1 if n is a prime power (including n = 1).  This implementation
    factorizes n with simple trial division, which is fast enough for
    n up to about 10 000 (the usual limits for the contest). */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;

    /* Find the smallest prime factor of m. */
    uint64_t p1;
    if ((m & 1) == 0)          /* divisible by 2 */
        p1 = 2;
    else {
        p1 = m;                /* assume m is prime */
        for (uint64_t p = 3; p <= m / p; p += 2) {
            if (m % p == 0) {
                p1 = p;
                break;
            }
        }
    }

    /* If m itself is prime, the sequence value is 1. */
    if (p1 == m)
        return (seq_t)1;

    /* Remove all powers of the first prime factor. */
    while (m % p1 == 0)
        m /= p1;

    /* If the remainder is 1, m was a prime power. */
    if (m == 1)
        return (seq_t)1;

    /* Find the smallest prime factor of the remaining part: the
       second distinct prime factor. */
    uint64_t p2;
    if ((m & 1) == 0)
        p2 = 2;
    else {
        p2 = m;
        for (uint64_t p = 3; p <= m / p; p += 2) {
            if (m % p == 0) {
                p2 = p;
                break;
            }
        }
    }

    return (seq_t)p2;
}
