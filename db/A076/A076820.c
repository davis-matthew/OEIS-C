/*
 * A076820  Second-largest distinct prime dividing n (or 1 if n is a power of
 *          a prime).
 * https://oeis.org/A076820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = the second–largest distinct prime factor of n, or 1 if n
 *            is a prime power (including n = 1)
 * Range:     u8 n <= 9483, u16 n <= 9483, u32 n <= 9483, u64 n <= 9483,
 *            u128 n <= 9483, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) {
        return 1;
    }

    /* factor_u64 returns the distinct prime factors of n in the array `pr` */
    uint64_t primes[32], exps[32];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt <= 1) {
        /* n has at most one distinct prime factor → prime power */
        return 1;
    }

    uint64_t largest = 0, second = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        if (p > largest) {
            second = largest;
            largest = p;
        } else if (p > second) {
            second = p;
        }
    }
    return (seq_t)second;
}
