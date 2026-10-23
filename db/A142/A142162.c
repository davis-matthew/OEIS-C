/*
 * A142162  Primes congruent to 5 mod 39.
 * https://oeis.org/A142162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime congruent to 5 modulo 39
 * Range:     u8 n <= 3, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate over all integers of the form 39*k+5, test each for primality,
   and return the n-th prime found. This works for any seq_t (including arbitrary–precision),
   because no dynamic memory of type seq_t is allocated. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    uint64_t candidate = 5;             /* first integer ≡5 (mod 39) */
    idx_t    count     = 0;             /* how many primes found so far */

    while (count < n) {
        if (is_prime(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* cast to seq_t (works for built‑in or
                                             * arbitrary‑precision types) */
        }
        /* Move to next number ≡5 (mod 39) */
        candidate += 39;
    }

    /* Should never reach here */
    return 0;
}
