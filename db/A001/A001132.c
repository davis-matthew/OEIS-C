/*
 * A001132  Primes == +-1 (mod 8).
 * https://oeis.org/A001132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search over odd numbers, counting the primes p with p == 1 or 7
 *            (mod 8)
 * Range:     u8 n <= 24, u16 n <= 3263, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1132_prime_check
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/a1132_prime_check.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t candidate = 7;

    for (;;) {
        uint64_t r = candidate % 8;
        if ((r == 1 || r == 7) && a1132_prime_check(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 2;
    }
}
