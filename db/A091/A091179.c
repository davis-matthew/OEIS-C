/*
 * A091179  A088878 indexed by A000040.
 * https://oeis.org/A091179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that A000040(k) = A088878(n); search: walk the
 *            primes p = A000040(k) in order and count those for which 3p-2 is
 *            also prime
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9527 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;   /* index of p among the primes */
    uint64_t p;
    for (p = 2; ; p++) {
        if (!is_prime_u64(p))
            continue;
        ++k;
        if (is_prime_u64(3 * p - 2)) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
