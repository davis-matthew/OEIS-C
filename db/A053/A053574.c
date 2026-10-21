/*
 * A053574  Exponent of 2 in phi(n) where phi(n) = A000010(n).
 * https://oeis.org/A053574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of 2 in φ(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    uint64_t x = n;
    unsigned long long v2_phi = 0;

    /* contribution from the power of two in n */
    unsigned int k = 0;
    while ((x & 1ULL) == 0) {
        k++;
        x >>= 1;
    }
    if (k > 0)
        v2_phi += (k - 1);   /* φ(2^k) = 2^(k-1) */

    /* factor the remaining odd part */
    uint64_t t = x;
    for (uint64_t d = 3; d <= t / d; d += 2) {
        if (t % d == 0) {
            /* d is a (odd) prime factor of n */
            uint64_t y = d - 1;
            while ((y & 1ULL) == 0) {
                v2_phi++;
                y >>= 1;
            }
            while (t % d == 0)
                t /= d;
        }
    }
    if (t > 1) {   /* remaining prime factor > sqrt(n) */
        uint64_t y = t - 1;
        while ((y & 1ULL) == 0) {
            v2_phi++;
            y >>= 1;
        }
    }

    return (seq_t)v2_phi;
}
