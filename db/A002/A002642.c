/*
 * A002642  Numbers k such that (k^2 + k + 1)/13 is prime.
 * https://oeis.org/A002642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search over k == 3 or 9 (mod 13), counting k with (k^2+k+1)/13
 *            prime (deterministic Miller-Rabin)
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a002642_isprime, a002642_mulmod, a002642_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a002642_isprime.h"
#include "../utils/a002642_mulmod.h"
#include "../utils/a002642_powmod.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 3;
    int phase = 0;

    for (;;) {
        uint64_t p = (k * k + k + 1) / 13;
        if (a002642_isprime(p)) {
            count++;
            if (count == n) return (seq_t)k;
        }
        k += phase == 0 ? 6 : 7;
        phase = 1 - phase;
    }
}
