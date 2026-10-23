/*
 * A142566  Primes congruent to 36 mod 53.
 * https://oeis.org/A142566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime congruent to 36 mod 53 (search)
 * Range:     u8 n <= 1, u16 n <= 108, u32 n <= 109, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    idx_t k     = 1;
    seq_t candidate;

    for (;;) {
        candidate = (seq_t)53 * (seq_t)k + 36;
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++k;
    }
}
