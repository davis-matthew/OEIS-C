/*
 * A142017  Primes congruent to 13 mod 31.
 * https://oeis.org/A142017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 222, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth prime congruent to 13 mod 31 by incremental search. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* below the offset – behaviour undefined */

    idx_t count = 0;
    uint64_t cand = 13;                 /* first candidate: 13 ≡ 13 (mod 31) */

    while (1) {
        if (is_prime(cand)) {
            count++;
            if (count == n) {
                return (seq_t)cand;    /* return as seq_t */
            }
        }
        cand += 31;                     /* next number ≡ 13 (mod 31) */
    }
}
