/*
 * A002327  Primes of the form k^2 - k - 1.
 * https://oeis.org/A002327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th prime of the form k^2 - k - 1 (k >= 2),
 *            tested with deterministic Miller-Rabin
 * Range:     u8 n <= 11, u16 n <= 105, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 63 terms through n = 8861 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k;
    for (k = 2; ; k++) {
        uint64_t v = k * (k - 1) - 1;
        if (is_prime_u64(v)) {
            count++;
            if (count == n) return (seq_t)v;
        }
    }
}
