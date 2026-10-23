/*
 * A048146  Sum of non-unitary divisors of n.
 * https://oeis.org/A048146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 396, u32 n <= 9987, u64 n <= 9987,
 *            u128 n <= 9987, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sigma(n) - usigma(n)
 * where sigma(n)   = sum of all positive divisors of n,
 *       usigma(n)  = sum of unitary divisors of n.
 * A unitary divisor d of n satisfies gcd(d, n/d) = 1.
 * For n = p1^e1 * p2^e2 * ... * pk^ek,
 *   sigma(n)    = ∏ (p_i^(e_i+1) - 1) / (p_i - 1)
 *   usigma(n)   = ∏ (1 + p_i^e_i)
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* factorization of n */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma   = 1;  /* product over primes of (p^{e+1} - 1)/(p-1) */
    seq_t unitary = 1;  /* product over primes of (1 + p^e)           */

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p  = (seq_t)primes[i];
        seq_t e  = (seq_t)exps[i];

        /* term for sigma: (p^{e+1} - 1) / (p - 1) */
        seq_t pow_pe_plus1 = big_pow(p, e + (seq_t)1);
        seq_t term_sigma   = (pow_pe_plus1 - (seq_t)1) / (p - (seq_t)1);
        sigma *= term_sigma;

        /* term for unitary divisors: 1 + p^e */
        seq_t pow_pe = big_pow(p, e);
        unitary *= (pow_pe + (seq_t)1);
    }

    return sigma - unitary;
}
