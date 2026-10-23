/*
 * A157188  Number of ways to write prime(n) as p*q-(p+q) with primes p<=q.
 * https://oeis.org/A157188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts pairs of primes p ≤ q with prime(n) = p*q-(p+q)
 * Range:     u8 n <= 54, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   get_prime, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to (p-1)(q-1) = prime(n)+1. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, but harmless */

    /* get the n‑th prime (1‑based indexing) */
    uint64_t pn = get_prime(n);    /* pn ≤ ~1e5 for n ≤ 10000 */
    uint64_t m  = pn + 1;          /* m = (p-1)(q-1) */

    seq_t count = 0;

    /* iterate over divisors d of m, d ≤ sqrt(m) */
    for (uint64_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t p = d + 1;
            if (is_prime_small((seq_t)p)) {
                uint64_t e = m / d;
                uint64_t q = e + 1;
                if (is_prime_small((seq_t)q)) {
                    /* p ≤ q automatically holds because d ≤ e */
                    count++;
                }
            }
        }
    }

    return count;
}
