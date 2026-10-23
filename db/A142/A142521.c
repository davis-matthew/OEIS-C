/*
 * A142521  Primes congruent to 29 mod 52.
 * https://oeis.org/A142521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 29 (mod 52) by straightforward trial
   division.  The function works for any value of `n` ≥ 1 and is suitable
   for the build that uses the provided `is_prime` helper. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n = 0 is outside the OEIS offset; return 0 as a safe placeholder. */
    if (n == 0) return (seq_t)0;

    idx_t candidate = 29;   /* first number ≡ 29 (mod 52) */
    idx_t count    = 0;

    while (1) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        candidate += 52;
    }
}
