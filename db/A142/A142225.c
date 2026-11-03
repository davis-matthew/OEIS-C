/*
 * A142225  Primes congruent to 28 mod 41.
 * https://oeis.org/A142225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 28 modulo 41
 * Range:     u8 n <= 2, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 911 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    uint64_t p = 2;

    while (found < n) {
        if ((p % 41ULL) == 28ULL) {
            ++found;
            if (found == n)
                return (seq_t)p;
        }
        p = next_prime(p);
    }
    /* unreachable, but keep the compiler happy */
    return 0;
}
