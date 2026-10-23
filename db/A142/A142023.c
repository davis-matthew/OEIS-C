/*
 * A142023  Primes congruent to 19 mod 31.
 * https://oeis.org/A142023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 207, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the nth prime ≡ 19 (mod 31) */
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t  candidate = 19;     /* the first number ≡ 19 (mod 31) */
    idx_t  count    = 0;       /* how many primes we have seen */

    while (true) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 31;
    }
}
