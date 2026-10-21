/*
 * A215360  Primes congruent to {0, 2, 3, 4} mod 17.
 * https://oeis.org/A215360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A215360: primes congruent to {0, 2, 3, 4} (mod 17).
   The function returns the n‑th such prime by scanning integers
   and testing for primality. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* declares bool is_prime(idx_t); */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset handling, not required for valid n */
    idx_t found = 0;
    uint64_t candidate = 2;               /* start from the first prime */

    while (1) {
        uint64_t r = candidate % 17;
        if (r == 0 || r == 2 || r == 3 || r == 4) {
            if (is_prime(candidate)) {
                ++found;
                if (found == n) return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
