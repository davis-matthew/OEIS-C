/*
 * A142433  Primes congruent to 23 mod 49.
 * https://oeis.org/A142433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p ≡ 23 (mod 49)
 * Range:     u8 n <= 1, u16 n <= 123, u32 n <= 137, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset, guard just in case */

    seq_t cand = 23;          /* first number 23 mod 49 */
    idx_t count = 0;

    for (;;)
    {
        if (is_prime_big(cand))
        {
            ++count;
            if (count == n)
                return cand;
        }
        cand += 49;
    }
}
