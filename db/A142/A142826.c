/*
 * A142826  Primes congruent to 28 mod 61.
 * https://oeis.org/A142826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p such that p ≡ 28 (mod 61)
 * Range:     u8 n <= 2, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;
    idx_t count = 0;
    idx_t candidate = 89;           /* 89 ≡ 28 (mod 61) and is the first such prime */
    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 61;             /* next number congruent to 28 (mod 61) */
    }
}
