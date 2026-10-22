/*
 * A002383  Primes of form k^2 + k + 1.
 * https://oeis.org/A002383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th prime of the form k^2 + k + 1, k >= 1
 *            (deterministic Miller-Rabin test)
 * Range:     u8 n <= 9, u16 n <= 66, u32 n <= 7364, u64 n <= 9531,
 *            u128 n <= 9531, big any n
 * Helpers:   a2383_isprime, a2383_mulmod, a2383_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2383_isprime.h"
#include "../utils/a2383_mulmod.h"
#include "../utils/a2383_powmod.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t k = 0;
    for (;;) {
        k++;
        uint64_t c = k * k + k + 1;
        if (a2383_isprime(c)) {
            count++;
            if (count == n) return (seq_t)c;
        }
    }
}
