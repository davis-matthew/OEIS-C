/*
 * A141996  Primes congruent to 20 mod 29.
 * https://oeis.org/A141996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 232, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 998 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the primes congruent to 20 modulo 29 (OEIS A141996).
   The sequence starts at n=1 with a(1)=107.  For general n we
   incrementally test candidates 20, 49, 78, … (each congruent to 20 mod 29)
   until the desired count is reached.  We use the provided
   is_prime_small function for primality testing. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, return 0 */
    idx_t count = 0;
    seq_t cand = 20;                 /* first number ≡20 (mod 29) */
    while (1) {
        cand += 29;                  /* next candidate ≡20 (mod 29) */
        if (is_prime_seq(cand)) {
            ++count;
            if (count == n) return cand;
        }
    }
}
