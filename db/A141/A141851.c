/*
 * A141851  Primes congruent to 4 mod 11.
 * https://oeis.org/A141851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 652, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 4 (mod 11) by incremental search
   using the supplied is_prime function. */
#include "../utils/is_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* index 0 is outside the defined offset; return 0 as a placeholder */
        return (seq_t)0;
    }

    /* The first prime ≡ 4 (mod 11) is 37. */
    seq_t prime = 37;
    if (n == 1) {
        return prime;
    }

    idx_t found = 1;          /* count of primes found so far */
    for (;;) {
        prime += 11;          /* next candidate with residue 4 mod 11 */
        if (is_prime((idx_t)prime)) {
            ++found;
            if (found == n) {
                return prime; /* desired term */
            }
        }
    }
}
