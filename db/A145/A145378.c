/*
 * A145378  a(n) = Sum_{d|n} sigma(d) - 2*Sum_{2c|n} sigma(c) + 4*Sum_{4b|n}
 *          sigma(b).
 * https://oeis.org/A145378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Σ_{d|n} σ(d) – 2 Σ_{2c|n} σ(c) + 4 Σ_{4b|n} σ(b)
 *            Equivalent multiplicative form: a(1) = 1 a(2^e) = 2^(e+2) – 3
 *            (e+1) a(p^e) = [p (p^(e+1) – 1) – (p–1)(e+1)]/(p–1)^2 (p > 2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 5416, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* simple integer power for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not used but safe   */
    if (n == 1) return 1;           /* base case          */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        idx_t e = (idx_t)exps[i];
        seq_t val;

        if (p == 2)                /* a(2^e) */
        {
            seq_t p2 = pow_seq((seq_t)2, e + 2);
            val = p2 - (seq_t)3 * (seq_t)(e + 1);
        }
        else                        /* a(p^e) for odd p */
        {
            seq_t p_pow = pow_seq((seq_t)p, e + 1);
            seq_t term1 = (seq_t)p * (p_pow - 1);
            seq_t term2 = (seq_t)(p - 1) * (seq_t)(e + 1);
            seq_t numer = term1 - term2;
            seq_t denom = (seq_t)(p - 1) * (seq_t)(p - 1);
            val = numer / denom;
        }

        res *= val;
    }

    return res;
}
