/*
 * A017670  Denominator of sum of -3rd powers of divisors of n.
 * https://oeis.org/A017670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{d|n} 1/d^3
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1626, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n, computes σ₃(n) as a reduced rational, then
   reduces n³/σ₃(n) to its lowest‑term denominator.  All intermediate values
   are kept in unsigned __int128 to avoid overflow for n ≤ 10000.  The
   function works for any build where seq_t is at least 64 bits.  For builds
   with larger seq_t (128‑bit or arbitrary precision) the cast at the end
   is still safe because the final denominator fits in 128 bits for all
   n ≤ 10000. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* --------------------------------------------------------------- */
/* auxiliary functions                                               */
/* --------------------------------------------------------------- */

/* fast exponentiation in 128‑bit */
static inline unsigned __int128 pow128(unsigned __int128 base, unsigned int exp)
{
    unsigned __int128 result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* Euclidean gcd for 128‑bit integers */
static inline unsigned __int128 gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b)
    {
        unsigned __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* --------------------------------------------------------------- */
/* sequence function                                               */
/* --------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Special case n == 1 */
    if (n == 1) return (seq_t)1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned __int128 sigma_num = 1;   /* numerator of σ₃(n) */
    unsigned __int128 sigma_den = 1;   /* denominator of σ₃(n) */

    for (size_t i = 0; i < cnt; ++i)
    {
        unsigned __int128 p   = (unsigned __int128)primes[i];
        unsigned int    e    = (unsigned int)exps[i];

        unsigned __int128 num_fac = pow128(p, 3U * (e + 1U)) - (unsigned __int128)1;
        unsigned __int128 den_fac = pow128(p, 3U)             - (unsigned __int128)1;

        sigma_num *= num_fac;
        sigma_den *= den_fac;

        /* Reduce immediately to keep numbers small */
        unsigned __int128 g = gcd128(sigma_num, sigma_den);
        sigma_num /= g;
        sigma_den /= g;
    }

    /* σ₃(n) is now in lowest terms: sigma_num / sigma_den */
    /* Compute n³ with 128‑bit to avoid overflow */
    unsigned __int128 n_big = (unsigned __int128)n;
    unsigned __int128 n3    = n_big * n_big * n_big;

    /* The denominator we need is n³ / gcd(n³, σ₃(n))   */
    unsigned __int128 g = gcd128(n3, sigma_num);
    unsigned __int128 denom = n3 / g;

    return (seq_t)denom;
}
