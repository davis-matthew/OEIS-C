/*
 * A142562  Primes congruent to 32 mod 53.
 * https://oeis.org/A142562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p ≡ 32 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 122, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */

    idx_t count = 0;
    idx_t candidate = 32;   /* first number ≡ 32 (mod 53) */

    while (count < n) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        candidate += 53;    /* move to next ≡ 32 (mod 53) candidate */
    }

    return 0; /* unreachable, but keeps the compiler happy */
}
