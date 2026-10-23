/*
 * A126773  a(n) = largest divisor of n which is coprime to the largest proper
 *          divisor of n. (a(1)=1.).
 * https://oeis.org/A126773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1. For n >= 2: Let p = the smallest prime divisor of n.
 *            If p^2 divides n, then a(n) = 1; else a(n) = p
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }

    uint64_t nn = (uint64_t)n;
    uint64_t p  = 0;

    /* Find the smallest prime divisor by trial division up to sqrt(nn). */
    for (uint64_t d = 2; d * d <= nn; ++d) {
        if (nn % d == 0) {
            p = d;
            break;
        }
    }
    if (p == 0) {          /* nn is prime */
        p = nn;
    }

    /* If p^2 divides nn, answer is 1; otherwise the divisor is p. */
    if (nn % (p * p) == 0) {
        return (seq_t)1;
    } else {
        return (seq_t)p;
    }
}
