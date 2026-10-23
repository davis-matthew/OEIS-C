/*
 * A045312  Primes congruent to {2, 3, 5, 6} (mod 7).
 * https://oeis.org/A045312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 4374, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to {2,3,5,6} modulo 7 by sequential search. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1, return 0 as sentinel */

    idx_t count = 0;
    idx_t candidate = 2;

    while (1) {
        if (is_prime(candidate)) {     /* fast deterministic primality test for 64‑bit values */
            idx_t mod = candidate % 7;
            if (mod == 2 || mod == 3 || mod == 5 || mod == 6) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
