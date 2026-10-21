/*
 * A061241  Prime numbers == 7 (mod 9).
 * https://oeis.org/A061241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 7 (mod 9)
 * Range:     u8 n <= 8, u16 n <= 1087, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Sequence starts at n = 1. The argument n is valid only for n ≥ 1. */
    if (n == 0) return 0;                /* offset not defined */

    seq_t candidate = 7;                  /* first candidate ≡ 7 (mod 9) */
    idx_t  found    = 0;                  /* number of primes found */

    while (true) {
        /* primality test for 64‑bit values is sufficient here */
        if (is_prime((idx_t)candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        candidate += 9;                   /* next number ≡ 7 (mod 9) */
    }
}
