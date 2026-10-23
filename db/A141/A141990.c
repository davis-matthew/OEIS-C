/*
 * A141990  Primes congruent to 14 mod 29.
 * https://oeis.org/A141990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 14 (mod 29)
 * Range:     u8 n <= 2, u16 n <= 238, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches successive numbers of the form 29*k+14
   and uses the provided primality routine is_prime() to test each
   candidate until the n‑th such prime is found. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* candidate = 29*k + 14 */
        idx_t cand = 29 * k + 14;
        if (is_prime(cand)) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
    }
}
