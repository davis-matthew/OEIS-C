/*
 * A215325  Primes congruent to {1, 2, 3, 4, 6} mod 19.
 * https://oeis.org/A215325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 1, 2, 3, 4 or 6 (mod 19)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Primitive primality test provided by the build utilities */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* 1‑based sequence – undefined for n < 1 */
        return (seq_t)0;

    idx_t count      = 0;
    idx_t candidate  = 2;      /* start with the first prime */

    for (;; ++candidate) {
        if (is_prime(candidate)) {
            idx_t r = candidate % 19;
            if (r == 1 || r == 2 || r == 3 || r == 4 || r == 6) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
    }
}
