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
 * Helpers:   a002327_isprime, a002327_mulmod, a002327_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a002327_isprime.h"
#include "../utils/a002327_mulmod.h"
#include "../utils/a002327_powmod.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k;
    for (k = 2; ; k++) {
        uint64_t v = k * (k - 1) - 1;
        if (a002327_isprime(v)) {
            count++;
            if (count == n) return (seq_t)v;
        }
    }
}
