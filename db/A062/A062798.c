/*
 * A062798  Inverse Moebius transform of A001405 (binomial(n, floor(n/2))).
 * https://oeis.org/A062798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(d, floor(d/2))
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 130,
 *            big any n
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/binomial.h"

/* Compute binomial(n, n/2) for  n ≤ 67 exactly using 128‑bit arithmetic. */
static seq_t binom_small(idx_t n)
{
    idx_t k = n >> 1;
    unsigned __int128 r = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        r = r * (n - k + i);
        r /= i;
    }
    return (seq_t)r;
}

/* Recursive helper that traverses all divisors of n. */
static void add_divisor_contrib(idx_t pos, idx_t cur,
                                const uint64_t *pr, const uint64_t *ex,
                                size_t cnt, seq_t *sum)
{
    if (pos == cnt)
    {
        seq_t term;
        if (sizeof(seq_t) > 8)                /* arbitrary‑precision build */
            term = binomial(cur, cur >> 1);
        else                                  /* 64‑bit build – skip huge terms */
            term = (cur <= 67) ? binom_small(cur) : (seq_t)0;

        *sum += term;
        return;
    }

    uint64_t p = pr[pos];
    uint64_t e = ex[pos];
    idx_t val = cur;

    /* exponent 0 : divisor unchanged */
    add_divisor_contrib(pos + 1, cur, pr, ex, cnt, sum);

    /* exponents 1 .. e */
    for (uint64_t i = 1; i <= e; ++i)
    {
        val *= (idx_t)p;
        add_divisor_contrib(pos + 1, val, pr, ex, cnt, sum);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Factor n. */
    size_t cnt = 0;
    uint64_t primes[64], exps[64];
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sum = 0;
    add_divisor_contrib(0, (idx_t)1, primes, exps, cnt, &sum);
    return sum;
}
