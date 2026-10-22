/*
 * A005383  Primes p such that (p+1)/2 is prime.
 * https://oeis.org/A005383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search over primes p, keeping those where (p+1)/2 is also prime
 * Range:     u8 n <= 8, u16 n <= 486, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    uint64_t p = 3;
    for (;;) {
        if (is_prime((p + 1) / 2) && is_prime(p)) {
            count++;
            if (count == n)
                return (seq_t)p;
        }
        p += 2;
    }
}
