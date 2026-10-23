/*
 * A127668  Concatenated indices of primes in prime factorization of n.
 * https://oeis.org/A127668
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of the 1‑based indices of the prime factors
 *            of n, listed from the largest prime to the smallest. For n =
 *            p1^e1 p2^e2 … pk^ek with p1 < p2 < … < pk, a(n) = pk…pk (e_k
 *            times) pk-1…pk-1 (e_{k-1} times) … p1…p1 (e_1 times)
 * Range:     u8 n <= 15, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   digit_len, factor_u64, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#include "../utils/factor_u64.h"   /* factor_u64 */
#include "../utils/get_prime.h"   /* get_prime */
#include "../utils/pow10.h"       /* pow10 */
#include "../utils/digit_len.h"   /* digit_len */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    /* Factor the integer n (ascending order of primes). */
    uint64_t primes[64] = {0};
    uint64_t exps[64]   = {0};
    size_t   cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)0;

    /* Process factors from right to left (largest to smallest prime). */
    for (int i = (int)cnt - 1; i >= 0; --i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Find the 1‑based index of prime p. */
        idx_t idx = 1;
        while ((uint32_t)get_prime(idx) < p)
            ++idx;
        /* Now get_prime(idx) == p. */

        /* pow10 of the number of decimal digits of idx. */
        int d = digit_len((seq_t)idx);
        seq_t pow10_d = pow10((idx_t)d);

        /* Append idx e times. */
        for (uint64_t rep = 0; rep < e; ++rep)
        {
            result = result * pow10_d + (seq_t)idx;
        }
    }

    return result;
}
