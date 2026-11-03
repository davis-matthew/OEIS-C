/*
 * A179307  a(n) = Sum_{d|n} C(n,d)*sigma(d)*sigma(n/d).
 * https://oeis.org/A179307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} C(n,d)*sigma(d)*sigma(n/d)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-01)
 * Helpers:   binomial, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

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

static void generate_divisors(const uint64_t *primes, const uint64_t *exps, size_t cnt,
                              size_t idx, uint64_t current, uint64_t *divs, size_t *dcnt)
{
    if (idx == cnt) {
        divs[*dcnt] = current;
        (*dcnt)++;
        return;
    }
    uint64_t p = primes[idx];
    uint64_t val = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        generate_divisors(primes, exps, cnt, idx + 1, current * val, divs, dcnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Factor n */
    const size_t MAX_FACTORS = 32;          /* enough for n <= UINT64_MAX */
    uint64_t primes[MAX_FACTORS];
    uint64_t exps[MAX_FACTORS];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate all divisors */
    uint64_t divisors[512];
    size_t dcnt = 0;
    generate_divisors(primes, exps, pcnt, 0, 1, divisors, &dcnt);

    seq_t total = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        seq_t sigma_d = sigma_of(d);
        uint64_t q = n / d;
        seq_t sigma_q = sigma_of(q);
        seq_t binom = binomial(n, d);  /* C(n,d) */
        seq_t tmp = binom * sigma_d;
        total += tmp * sigma_q;
    }
    return total;
}
