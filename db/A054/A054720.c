/*
 * A054720  Number of 5-ary sequences with primitive period n.
 * https://oeis.org/A054720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} μ(d) * 5^{n/d} (OEIS A054720)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 396 (2026-11-01)
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factorize.h"

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

static void rec_divisors(int i, uint64_t cur, int cnt, bool sqfree,
                         const uint64_t *primes, const int *exps, int pf_count,
                         idx_t n, seq_t *posSum, seq_t *negSum)
{
    if (i == pf_count)
    {
        if (sqfree)
        {
            seq_t term = power5(n / cur);   /* 5^{n/d} */
            if ((cnt & 1) == 0)   /* μ = +1 (even number of primes) */
                *posSum += term;
            else                  /* μ = -1 (odd number of primes) */
                *negSum += term;
        }
        return;
    }

    uint64_t p = primes[i];
    uint64_t c = cur;
    for (int e = 0; e <= exps[i]; ++e)
    {
        bool nsqfree = sqfree;
        int ncnt = cnt;
        if (e == 1)
            ++ncnt;
        else if (e > 1)
            nsqfree = false;

        rec_divisors(i + 1, c, ncnt, nsqfree, primes, exps,
                     pf_count, n, posSum, negSum);

        if (e < exps[i])
            c *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) /* by definition a(0) = 1 */
        return (seq_t)1;

    /* Factor n */
    uint64_t primes[32];
    int exps[32];
    int pf_count = factorize(n, primes, exps);

    seq_t posSum = 0;
    seq_t negSum = 0;

    rec_divisors(0, 1, 0, true,
                 primes, exps, pf_count,
                 n, &posSum, &negSum);

    seq_t result = posSum;
    result -= negSum;
    return result;
}
