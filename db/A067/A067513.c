/*
 * A067513  Number of divisors d of n such that d+1 is prime.
 * https://oeis.org/A067513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = number of divisors d of n such that d + 1 is prime (OEIS
 *            A067513). n ≥ 1. The function is written for general seq_t/idx_t
 *            types and uses the helper is_prime from "../utils/is_prime.h"
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 1; for any n ≥ 1 we compute the value. */
    seq_t count = 0;

    /* Walk up to sqrt(n).  Using 128‑bit multiplication avoids overflow. */
    for (idx_t i = 1; ( __uint128_t)i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            if (is_prime(i + 1))
                ++count;

            /* complementary divisor j = n / i */
            idx_t j = n / i;
            if (j != i && is_prime(j + 1))
                ++count;
        }
    }
    return count;
}
