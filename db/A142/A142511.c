/*
 * A142511  Primes congruent to 7 mod 52.
 * https://oeis.org/A142511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime congruent to 7 modulo 52
 * Range:     u8 n <= 3, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_generic.h"

seq_t a(idx_t n)
{
    /* Sequence is offset 1: for n==0 return 0 (undefined). */
    if (n == 0) return (seq_t)0;

    idx_t cnt = 0;          /* how many such primes have been found */
    idx_t cand = 7;         /* first integer ≡ 7 (mod 52) */

    for (;;) {
        if (is_prime_generic(cand)) {
            ++cnt;
            if (cnt == n) return (seq_t)cand;
        }
        cand += 52;           /* next candidate ≡ 7 (mod 52) */
    }
}
