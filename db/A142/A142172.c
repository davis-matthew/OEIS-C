/*
 * A142172  Primes congruent to 22 mod 39.
 * https://oeis.org/A142172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 277, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th prime ≡ 22 (mod 39) (a(1)=61, a(2)=139, …) */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    uint64_t candidate = 22;   /* current number congruent to 22 mod 39 */
    idx_t count = 0;

    /* Find the n‑th prime in this residue class */
    while (count < n) {
        candidate += 39;                   /* next candidate ≡ 22 (mod 39) */
        if (is_prime(candidate)) {          /* test for primality */
            ++count;
        }
    }

    return (seq_t)candidate;
}
