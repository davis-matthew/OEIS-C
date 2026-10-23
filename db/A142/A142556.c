/*
 * A142556  Primes congruent to 26 mod 53.
 * https://oeis.org/A142556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 26 modulo 53
 * Range:     u8 n <= 1, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cand = 79;                   /* first prime ≡ 26 (mod 53) */
    while (1) {
        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        cand += 53;
    }
}
