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
 * Validated: standard, pass, 116 terms through n = 8147 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static void _count_divs_leq(uint64_t *pr, uint64_t *exp, size_t pcnt,
                            size_t idx, seq_t cur, seq_t abundance,
                            size_t *cnt)
{
    if (idx == pcnt) {
        if (cur <= abundance) (*cnt)++;
        return;
    }
    seq_t val = cur;
    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        _count_divs_leq(pr, exp, pcnt, idx + 1, val, abundance, cnt);
        val *= (seq_t)pr[idx];
    }
}

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
