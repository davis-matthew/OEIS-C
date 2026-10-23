/*
 * A142166  Primes congruent to 11 mod 39.
 * https://oeis.org/A142166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 11 (mod 39)
 * Range:     u8 n <= 3, u16 n <= 276, u32 n <= 195, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based: a(1) = 11, a(2) = 89, … */
    seq_t candidate = (seq_t)11;          /* first number ≡ 11 (mod 39) */
    idx_t   count   = 0;

    while (1) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += (seq_t)39;           /* next candidate in the progression */
    }
}
