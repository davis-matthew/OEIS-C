/*
 * A142553  Primes congruent to 23 mod 53.
 * https://oeis.org/A142553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 130, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime congruent to 23 (mod 53).  The sequence
   A142553 lists these primes with a(1) = 23. */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 1) return (seq_t)23;

    /* Start from the first term and search forwards */
    uint64_t cand = 23;           /* current prime candidate */
    idx_t    count = 1;           /* how many such primes found */

    while (count < n) {
        cand += 53;               /* next integer in the congruence class 23 mod 53 */
        if (is_prime((idx_t)cand)) {
            count++;             /* new prime found */
        }
    }

    return (seq_t)cand;
}
