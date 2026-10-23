/*
 * A142543  Primes congruent to 13 mod 53.
 * https://oeis.org/A142543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 126, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A142543: primes congruent to 13 modulo 53. */
/* a(n) is the n‑th prime p such that p % 53 == 13, with a(1) = 13. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Assumes n >= 1; behavior for n == 0 is unspecified. */
    idx_t count = 0;
    idx_t candidate = 13;            /* the first number with residue 13 mod 53 */

    while (true)                    /* run until we find the n-th such prime */
    {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* cast to the build‑specific seq_t */
        }
        candidate += 53;            /* move to the next number with the same residue */
    }
}
