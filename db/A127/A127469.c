/*
 * A127469  a(n) = n * A062949(n).
 * https://oeis.org/A127469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = n * A062949(n) = n * Π_{p^e||n}
 *            ((e+1)*p^(2*e+1)-(e+2)*p^(2*e)+p^e)/(p-1)
 * Range:     u8 n <= 9, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Computes the value of a prime power contribution a(p^e) using the
   multiplicative formula given in the OEIS entry.  The intermediate
   computation is performed in unsigned 128‑bit arithmetic to avoid
   overflow for the 64‑bit build.  It works for any seq_t that can be
   constructed from an unsigned __int128. */
static seq_t prime_power_val(uint64_t p, unsigned int e)
{
    unsigned __int128 P = 1;
    for (unsigned int i = 0; i < e; ++i)
        P *= p;                      /* P = p^e */

    unsigned __int128 P2 = P * P;     /* p^(2e) */
    unsigned __int128 numer =
        (unsigned __int128)(e + 1) * (P2 * p)    /* (e+1)*p^(2e+1) */
        - (unsigned __int128)(e + 2) * P2       /* (e+2)*p^(2e) */
        + P;                                   /* + p^e */

    unsigned __int128 denom = (unsigned __int128)(p - 1);
    unsigned __int128 res = numer / denom;      /* exact division */

    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1, but return 0 for 0 to make function safe */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        /* convert exponent to unsigned int for convenience */
        unsigned int e = (unsigned int)exps[i];
        seq_t part = prime_power_val(primes[i], e);
        result *= part;
    }
    return result;
}
