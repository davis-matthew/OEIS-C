/*
 * A142254  Primes congruent to 5 mod 43.
 * https://oeis.org/A142254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 5 (mod 43)
 * Range:     u8 n <= 1, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation scans successive numbers ≡ 5 mod 43 and returns
   the n-th one that is prime. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 5;           /* first positive integer ≡ 5 mod 43 */

    for (;; candidate += 43) {      /* loop over all numbers ≡ 5 (mod 43) */
        if (is_prime(candidate)) { /* only those that are prime count */
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
