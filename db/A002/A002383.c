/*
 * A002383  Primes of form k^2 + k + 1.
 * https://oeis.org/A002383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th prime of the form k^2 + k + 1, k >= 1
 *            (deterministic Miller-Rabin test)
 * Range:     u8 n <= 9, u16 n <= 66, u32 n <= 7364, u64 n <= 10751,
 *            u128 n <= 10751, big any n
 * Validated: standard, pass, 61 terms through n = 9531 (2026-11-03)
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
    uint64_t k = 0;
    for (;;) {
        k++;
        uint64_t c = k * k + k + 1;
        if (is_prime_u64(c)) {
            count++;
            if (count == n) return (seq_t)c;
        }
    }
}
