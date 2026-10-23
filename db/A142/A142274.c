/*
 * A142274  Primes congruent to 25 mod 43.
 * https://oeis.org/A142274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 131, u32 n <= 132, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 25 (mod 43) */
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based index; find the nth prime ≡ 25 (mod 43) */
    idx_t found = 0;
    uint64_t candidate = 25;   /* first positive integer with that residue */

    while (true) {
        if (is_prime_big((seq_t)candidate)) {
            ++found;
            if (found == n) return (seq_t)candidate;
        }
        candidate += 43;
    }
}
