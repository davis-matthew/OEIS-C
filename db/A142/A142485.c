/*
 * A142485  Primes congruent to 14 mod 51.
 * https://oeis.org/A142485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 201, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 14 mod 51 */
/* a(n) is the n-th prime p such that p ≡ 14 (mod 51). */
/* The function performs a simple search, checking each candidate */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* candidate values that are ≡ 14 (mod 51) */
    idx_t candidate = 14;
    idx_t count = 0;

    while (1) {
        if (is_prime(candidate)) {
            count++;
            if (count == n) {
                return (seq_t)candidate;
            }
        }
        /* move to the next number in the progression 14, 65, 116, ... */
        candidate += 51;
    }
}
