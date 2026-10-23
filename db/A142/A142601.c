/*
 * A142601  Primes congruent to 1 mod 55.
 * https://oeis.org/A142601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the n-th prime congruent to 1 modulo 55
 * Range:     u8 none, u16 n <= 159, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1; behaviour below offset is not required */

    seq_t cand = 331;                  /* first prime ≡ 1 (mod 55) */
    idx_t  count = 0;

    while (1)
    {
        if (is_prime_big(cand))
        {
            ++count;
            if (count == n) return cand;
        }
        cand += (seq_t)55;             /* next candidate with the same modulus */
    }
}
