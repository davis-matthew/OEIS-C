/*
 * A142259  Primes congruent to 10 mod 43.
 * https://oeis.org/A142259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 10 (mod 43)
 * Range:     u8 n <= 2, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for n < 1, but never called with n=0 in tests */
    }

    seq_t count = 0;
    seq_t cand  = (seq_t)53; /* first prime ≡ 10 (mod 43) */

    for (;; cand += (seq_t)43) {
        if (is_prime((idx_t)cand)) {
            count++;
            if (count == (seq_t)n) {
                return cand;
            }
        }
    }
}
