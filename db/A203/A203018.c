/*
 * A203018  The n-th prime number that equals 1 (mod 4n).
 * https://oeis.org/A203018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p that satisfies p ≡ 1 (mod 4n) (n ≥ 1)
 * Range:     u8 n <= 5, u16 n <= 57, u32 n <= 42, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n-th term of sequence A203018.
   For n = 1,2,...   a(n) is the n-th prime that is congruent to 1 modulo 4n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* undefined, but defensive */

    /* Modulus m = 4n, computed in seq_t to avoid overflow. */
    const seq_t m = ((seq_t)n) * ((seq_t)4);

    idx_t   count = 0;                   /* how many qualifying primes found */
    idx_t   k     = 1;                    /* multiplier for the modulus */
    seq_t   result = 0;

    /* Search for k = 1, 2, … until we have found n primes in the progression. */
    for (;; ++k) {
        const seq_t p = m * (seq_t)k + 1;   /* candidate prime = 1 + 4n*k */
        if (is_prime_big(p)) {
            ++count;
            if (count == n) {
                result = p;
                break;
            }
        }
    }

    return result;
}
