/*
 * A023817  Sum of exponents in prime-power factorization of C(2n,n-1).
 * https://oeis.org/A023817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω( C(2n, n-1) ) = Σₚ≤2n (⌊2n/pᵏ⌋ – ⌊(n‑1)/pᵏ⌋ –
 *            ⌊(n+1)/pᵏ⌋)
 * Range:     u8 n <= 1187, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Upper bound for primes needed in the Legendre sums */
    idx_t limit = n << 1;                 /* 2 * n */

    /* Obtain all primes ≤ limit; the utility allocates the array. */
    uint64_t *primes = NULL;
    size_t prime_count = 0;
    build_primes_upto(limit, &primes, &prime_count);

    seq_t sum = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t p = primes[i];
        idx_t exp = 0;
        uint64_t power = p;

        while (power <= limit)
        {
            exp += limit   / power;
            exp -= (n - 1) / power;
            exp -= (n + 1) / power;

            /* Prevent overflow of power *= p */
            if (power > limit / p)
                break;
            power *= p;
        }
        sum += (seq_t)exp;
    }

    free(primes);
    return sum;
}
