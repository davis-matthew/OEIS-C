/*
 * A235027  Reverse the bits of prime divisors of n (with 2 -> 2), and
 *          multiply together: a(0)=0, a(1)=1, a(2)=2, a(p) = revbits(p) for
 *          odd primes p, a(u*v) = a(u) * a(v) for composites.
 * https://oeis.org/A235027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, mult
 * Formula:   a(n) = product of revbits(p) over all prime factors p of n,
 *            counting multiplicity. For odd primes revbits reverses the
 *            binary representation of p; revbits(0)=0, revbits(2)=2. a(0)=0,
 *            a(1)=1
 * Range:     u8 n <= 200, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   revbits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/revbits_uint64.h"

/* Completely multiplicative sequence A235027. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    seq_t result = (seq_t)1;

    while (tmp > 1) {
        uint64_t p;
        if ((tmp & 1) == 0) {
            p = 2;
        } else {
            p = 0;
            for (uint64_t d = 3; d * d <= tmp; d += 2) {
                if (tmp % d == 0) {
                    p = d;
                    break;
                }
            }
            if (p == 0) p = tmp;   /* tmp itself is odd prime */
        }
        result *= revbits_uint64(p);
        tmp /= p;
    }

    return result;
}
