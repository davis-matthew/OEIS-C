/*
 * A142550  Primes congruent to 20 mod 53.
 * https://oeis.org/A142550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 127, u32 n <= 78, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th prime congruent to 20 mod 53 by brute‑force search.
   The sequence a(n) is defined as the n-th prime p with p ≡ 20 (mod 53). */
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) {   /* offset starts at 1; return 0 or whatever for 0 */
        return 0;
    }
    seq_t cand = 20;          /* first candidate in the progression */
    idx_t  cnt  = 0;          /* count of primes found so far */

    while (1) {
        if (is_prime_big(cand)) {
            ++cnt;
            if (cnt == n) {
                return cand;
            }
        }
        cand += 53;           /* next number ≡ 20 (mod 53) */
    }
}
