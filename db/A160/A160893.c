/*
 * A160893  a(n) = Sum_{d|n} Möbius(n/d)*d^5/phi(n).
 * https://oeis.org/A160893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_5(n)/phi(n) = ∏_{p^e‖n} p^(4e-4) * (1 + p + p^2 + p^3 +
 *            p^4) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 197, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* n = 0 is not defined for the sequence; return 0 as a safety value */
    if (n == 0) return 0;

    seq_t result = 1;              /* multiplicative identity in seq_t space */
    uint64_t temp = (uint64_t)n;   /* temporary copy for factorisation */

    /* trial division up to sqrt(temp) */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* exponent of p in n */
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            /* compute p^(4*(e-1))  (for e=1 this is 1) */
            seq_t p_pow = 1;
            for (int i = 0; i < 4 * (e - 1); ++i)
                p_pow *= (seq_t)p;

            /* compute geometric sum 1 + p + p^2 + p^3 + p^4 */
            seq_t sum = 1;
            seq_t term = 1;
            for (int i = 0; i < 4; ++i)
            {
                term *= (seq_t)p;
                sum += term;
            }

            /* multiply the prime‑power contribution into the result */
            result *= p_pow * sum;
        }
    }

    /* remaining prime factor > sqrt(original n) (if any) */
    if (temp > 1)
    {
        uint64_t p = temp;          /* exponent e = 1 for this prime */
        seq_t p_pow = 1;            /* p^(4*(1-1)) = 1 */
        seq_t sum = 1;
        seq_t term = 1;
        for (int i = 0; i < 4; ++i)
        {
            term *= (seq_t)p;
            sum += term;
        }
        result *= p_pow * sum;
    }

    return result;
}
