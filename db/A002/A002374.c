/*
 * A002374  Largest prime <= n in any decomposition of 2n into a sum of two
 *          odd primes.
 * https://oeis.org/A002374
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = largest prime p <= n such that 2n - p is also an odd
 *            prime (search downward from n)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a002374_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/a002374_prime.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    uint64_t two_n = 2 * m;
    uint64_t p = (m % 2 == 0) ? m - 1 : m;
    while (p >= 3) {
        uint64_t q = two_n - p;
        if (q >= 3 && a002374_prime(p) && a002374_prime(q)) {
            return (seq_t)p;
        }
        p -= 2;
    }
    return (seq_t)3;
}
