/*
 * A059308  Primes p such that x^15 = 2 has no solution mod p.
 * https://oeis.org/A059308
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Prime p such that the congruence x^15 ≡ 2 (mod p) has no
 *            solution
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 960 (2026-11-01)
 * Helpers:   is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Value a(n) is the nth such prime (OEIS offset 1). */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t candidate = 2;
    while (true) {
        /* Only test odd primes ≥ 7 (2,3,5 are not in the sequence). */
        if (candidate >= 7 && is_prime(candidate)) {
            uint64_t dm1 = candidate - 1;
            uint64_t g;
            if (dm1 % 3 == 0) {
                if (dm1 % 5 == 0) g = 15;
                else                g = 3;
            } else if (dm1 % 5 == 0) {
                g = 5;
            } else {
                g = 1;
            }
            uint64_t exp = dm1 / g;
            seq_t res = powmod((seq_t)2, exp, (seq_t)candidate);
            if (res != (seq_t)1) {
                count++;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        /* Advance to the next odd candidate. */
        if (candidate == 2)
            candidate = 3;
        else
            candidate += 2;
    }
}
