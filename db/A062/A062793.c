/*
 * A062793  Moebius transform of n^n.
 * https://oeis.org/A062793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} mu(d) * (n/d)^(n/d)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 384 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

static int mu_of_divisor(uint64_t d, const uint64_t *primes, size_t pcnt)
{
    int mu = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > d) break;
        if (d % p == 0) {
            d /= p;
            if (d % p == 0) return 0;      /* square factor found */
            mu = -mu;                      /* toggle sign for each prime */
        }
    }
    if (d > 1) mu = -mu;                     /* remaining prime factor */
    return mu;
}

seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 (won't be used) */
    if (n == 0) return 0;

    /* Factorize n to obtain prime bases and their exponents for divisor generation. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* Generate all divisors of n.  For n ≤ 10 000 the divisor count is < 200.  */
    uint64_t divisors[512];
    size_t dcnt = 1;
    divisors[0] = 1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        int e = (int)exps[i];
        size_t cur = dcnt;
        uint64_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= p;
            for (size_t j = 0; j < cur; ++j) {
                divisors[dcnt++] = divisors[j] * p_pow;
            }
        }
    }

    /* Compute the Möbius transform sum. */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        idx_t k = (idx_t)(n / d);            /* n/d fits in idx_t (≤ n) */
        seq_t term = ipow((seq_t)k, (seq_t)k);   /* (n/d)^(n/d) */
        int mu = mu_of_divisor(d, primes, pcnt);
        if (mu == 1)        sum += term;
        else if (mu == -1)  sum -= term;
        /* mu == 0: skip */
    }
    return sum;
}
