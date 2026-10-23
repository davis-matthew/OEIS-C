/*
 * A142212  Primes congruent to 15 mod 41.
 * https://oeis.org/A142212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation of primes ≡ 15 (mod 41). */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* n starts at 1 in OEIS offset */

    idx_t found  = 0;
    idx_t k      = 1;              /* k such that candidate = 15 + 41*k */

    for (;; ++k) {
        idx_t candidate = 15 + 41 * k;   /* always ≡ 15 (mod 41) */

        if (is_prime(candidate)) {
            ++found;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
    }
}
