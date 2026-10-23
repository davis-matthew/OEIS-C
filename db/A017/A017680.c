/*
 * A017680  Denominator of sum of -8th powers of divisors of n.
 * https://oeis.org/A017680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum of -8th powers of divisors of n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 255,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is a(n) = n^8 / gcd( sigma_8(n), n^8 ) where sigma_8(n) = sum_{d|n} d^8  */
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_gcd.h"

static seq_t compute_denominator(idx_t n)
{
    /* factor n into primes and exponents */
    const size_t MAX_FACTORS = 64;
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* compute sigma_8(n) = ∏_p (1 + p^8 + p^16 + ... + p^{8e}) */
    seq_t sigma = 1;
    for (size_t i = 0; i < pcnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t p8 = seq_pow(p, 8);                /* p^8 */
        seq_t term = 1;                          /* current p^{8k} , starts with 1 for k=0 */
        seq_t sum = 1;                           /* sum of powers for this prime factor */

        for (uint64_t e = 1; e <= exps[i]; ++e)
        {
            term = term * p8;                    /* term = p^{8e} */
            sum = sum + term;                    /* add to the sum */
        }
        sigma = sigma * sum;                     /* multiply into overall sigma */
    }

    /* compute n^8 */
    seq_t n8 = seq_pow((seq_t)n, 8);

    /* denominator = n^8 / gcd( sigma, n^8 ) */
    seq_t g = seq_gcd(sigma, n8);
    return n8 / g;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* undefined for n=0, return 0 as safety */
    return compute_denominator(n);
}
