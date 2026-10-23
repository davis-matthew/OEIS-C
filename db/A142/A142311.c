/*
 * A142311  Primes congruent to 43 mod 44.
 * https://oeis.org/A142311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 43 modulo 44
 * Range:     u8 n <= 2, u16 n <= 330, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search using the helper is_prime(idx_t). */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* number of qualifying primes found so far */
    idx_t candidate = 43;     /* first number ≡ 43 (mod 44) */

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        candidate += 44;     /* next candidate with the same residue */
    }
}
