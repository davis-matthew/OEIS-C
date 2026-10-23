/*
 * A142219  Primes congruent to 22 mod 41.
 * https://oeis.org/A142219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 22 mod 41 (search)
 * Range:     u8 n <= 1, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* offset starts at 1, below offset is irrelevant */
    idx_t count = 0;
    idx_t candidate = 22;              /* first number ≡ 22 (mod 41) */
    while (1) {
        if (candidate >= 2 && is_prime(candidate)) {
            count++;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 41;               /* skip to next number ≡ 22 (mod 41) */
    }
}
