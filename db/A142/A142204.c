/*
 * A142204  Primes congruent to 7 mod 41.
 * https://oeis.org/A142204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 159, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th prime congruent to 7 (mod 41).
   For each n ≥ 1, returns the n‑th prime p with p ≡ 7 (mod 41). */
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n = 0, return 0 for safety */
        return 0;

    seq_t candidate = 7;   /* first prime in the progression */
    idx_t count     = 1;   /* number of primes found so far */

    if (n == 1)
        return candidate;

    for (;;) {
        candidate += 41;          /* move to the next number ≡ 7 (mod 41) */
        if (!isPrimeGeneric(candidate))
            continue;            /* skip composites */
        ++count;
        if (count == n)
            return candidate;
    }
}
