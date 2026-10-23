/*
 * A142193  Primes congruent to 27 mod 40.
 * https://oeis.org/A142193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 312, u32 n <= 380, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142193: primes congruent to 27 mod 40 */
/* The sequence is defined by: a(n) is the n-th prime p such that p % 40 == 27. */
/* Computation: iterate over candidates 27 + 40*k, test primality until the n-th such prime is found. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /*  Sequence starts at p = 67 (27 + 40).   */
    seq_t p = 27;
    idx_t count = 0;

    while (count < n) {
        p += 40;
        if (is_prime_big(p)) {
            ++count;
        }
    }

    return p;
}
