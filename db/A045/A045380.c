/*
 * A045380  Primes congruent to 2 mod 5.
 * https://oeis.org/A045380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 2 (mod 5) obtained by linear search. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Search candidates of the form 5k+2. */
    uint64_t candidate = 2;          /* first prime ≡ 2 (mod 5) */
    idx_t count = 0;

    for (;; candidate += 5) {
        seq_t cand = (seq_t)candidate;
        if (is_prime_big(cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
    }
}
