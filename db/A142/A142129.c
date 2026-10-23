/*
 * A142129  Primes congruent to 20 mod 37.
 * https://oeis.org/A142129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 145, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 20 mod 37
 * Formula: a(n) = the n‑th prime p such that p % 37 == 20   (n ≥ 1)
 * */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for n = 0 */

    seq_t cand = 20;                      /* start from the first number ≡20 (mod 37) */
    idx_t  cnt = 0;                        /* number of primes found so far */

    while (1) {
        if (is_prime_big(cand)) {          /* test primality */
            ++cnt;
            if (cnt == n)
                return cand;              /* return the n‑th prime */
        }
        cand += 37;                       /* next candidate ≡20 (mod 37) */
    }
}
