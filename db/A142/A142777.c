/*
 * A142777  Primes congruent to 50 mod 59.
 * https://oeis.org/A142777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 115, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 50 mod 59.  The nth term is the nth prime
   p such that p ≡ 50 (mod 59).  The function generates candidates
   59*k+50 and tests them for primality until the nth is found. */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Undefined for n < 1 */

    idx_t count = 0;
    idx_t k = 0;

    for (;; ++k) {
        idx_t cand = 59 * k + 50;
        if (cand < 2) continue;          /* skip the composite 50 */
        if (is_prime(cand)) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
