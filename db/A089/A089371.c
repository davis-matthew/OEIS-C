/*
 * A089371  Number of divisors of n that do not exceed the abundance of n.
 * https://oeis.org/A089371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d>0 : d | n and d ≤ abundance(n) } where abundance(n)
 *            = σ(n) − 2n, σ(n) = sum of divisors of n
 * Range:     u8 n <= 6170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _count_divs_leq, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/_count_divs_leq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* factorization of n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* divisor count τ(n) */
    seq_t divcnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        divcnt *= (seq_t)(exps[i] + 1);

    /* sum of divisors σ(n) */
    seq_t sigma = sigma_of((idx_t)n);

    /* two times n */
    seq_t two_n = (seq_t)2 * (seq_t)n;

    /* no positive abundance ⇒ no divisor qualifies */
    if (sigma <= two_n) return 0;

    /* abundance = σ(n) − 2n */
    seq_t abundance = sigma - two_n;

    /* abundance ≥ n ⇒ every divisor is ≤ abundance */
    if (abundance >= (seq_t)n) return divcnt;

    /* count divisors ≤ abundance by recursive enumeration */
    size_t cnt = 0;
    _count_divs_leq(primes, exps, pcnt, 0, (seq_t)1, abundance, &cnt);

    return (seq_t)cnt;
}
