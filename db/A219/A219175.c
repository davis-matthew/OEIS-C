/*
 * A219175  a(n) = n mod lambda(n) where lambda is the Carmichael function
 *          (A002322).
 * https://oeis.org/A219175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod lambda(n) where lambda is Carmichael's function
 * Range:     u8 n <= 457, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#include "../utils/gcd.h"
#include "../utils/pow_uint.h"
#include "../utils/factor_u64.h"

/* Compute Carmichael's lambda function for n (n >= 1) */
static seq_t carmichael(idx_t n)
{
    if (n <= 1)
        return 1; /* lambda(1) = 1 analytic convention */

    /* Temporary storage for prime power factorisation */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* Factorise n into primes: primes[i]^exps[i], i = 0..cnt-1 */
    factor_u64(n, primes, exps, &cnt);

    seq_t lambda = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t lam_i;
        if (primes[i] == 2ULL)
        {
            /* Carmichael values for 2^e */
            if (exps[i] == 1)
                lam_i = 1;                 /* lambda(2)  = 1   */
            else if (exps[i] == 2)
                lam_i = 2;                 /* lambda(4)  = 2   */
            else
                lam_i = ((seq_t)1) << (exps[i] - 2); /* 2^(e-2)      */
        }
        else
        {
            /* For odd prime powers */
            lam_i = pow_uint(primes[i], (idx_t)(exps[i] - 1));
            lam_i *= (seq_t)(primes[i] - 1);
        }

        /* lambda = lcm(lambda, lam_i) = lambda/gcd * lam_i */
        seq_t g = gcd(lambda, lam_i);
        lambda = (lambda / g) * lam_i;
    }

    return lambda;
}

/* OEIS sequence a(n) */
seq_t a(idx_t n)
{
    seq_t lam = carmichael(n);
    return (seq_t)n % lam;
}
