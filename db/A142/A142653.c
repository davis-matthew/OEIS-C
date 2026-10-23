/*
 * A142653  Primes congruent to 29 mod 56.
 * https://oeis.org/A142653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 29 modulo 56
 * Range:     u8 n <= 2, u16 n <= 227, u32 n <= 228, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n == 0 is out of range; return 0 in that case. */
    if (n == 0)
        return 0;

    /* Search for the n‑th prime p satisfying p ≡ 29 (mod 56). */
    seq_t candidate = 29;      /* first number in the residue class */
    idx_t   count    = 0;      /* how many primes found so far */

    while (1) {
        if (is_prime_big(candidate)) {
            ++count;
            if (count == n)
                return candidate;  /* found the desired prime */
        }
        candidate += 56;          /* move to the next number in the class */
    }
}
