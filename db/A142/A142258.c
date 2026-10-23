/*
 * A142258  Primes congruent to 9 mod 43.
 * https://oeis.org/A142258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 114, u32 n <= 115, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes congruent to 9 mod 43 (search). */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Candidate numbers of the form 43*k + 9. */
    seq_t candidate = (seq_t)9;
    idx_t count = 0;

    while (1) {
        /* Check primality. */
        if (is_prime_big(candidate)) {
            count++;
            if (count == n) {
                return candidate;
            }
        }
        /* Next candidate in the arithmetic progression. */
        candidate += (seq_t)43;
    }
}
