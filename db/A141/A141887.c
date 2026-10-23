/*
 * A141887  Primes congruent to 19 mod 20.
 * https://oeis.org/A141887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 19 (mod 20)
 * Range:     u8 n <= 7, u16 n <= 744, u32 n <= 780, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* offset starts at 1 – undefined for n < 1 */

    seq_t count = 0;            /* how many qualifying primes found so far */
    seq_t cand  = 19;           /* first candidate in the progression 20k–1 */

    while (count < n) {
        if (is_prime_big(cand)) {        /* test primality of the candidate */
            ++count;
            if (count == n) return cand; /* found the n-th prime */
        }
        cand += 20;                      /* next number ≡ 19 (mod 20) */
    }

    return cand;   /* unreachable, but keeps compiler happy */
}
