/*
 * A160400  a(n) is the smallest positive integer such that a(n)*n = j^k, for
 *          some j (j>=1) and k (k>=2).
 * https://oeis.org/A160400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m such that n*m is a perfect power j^k (k>=2)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Helper: fast exponentiation for any seq_t type. */
static inline seq_t power_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

/* Smallest positive integer m such that n*m is a perfect power. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Factorise n.  For n <= 10^4 the trial division is fast enough. */
    uint64_t primes[64];
    idx_t     exps[64];
    size_t    pf_cnt = 0, i;

    uint64_t temp = (uint64_t)n;
    if (temp % (uint64_t)2 == (uint64_t)0)
    {
        primes[pf_cnt] = (uint64_t)2;
        exps[pf_cnt]   = (idx_t)0;
        while (temp % (uint64_t)2 == (uint64_t)0)
        {
            temp /= (uint64_t)2;
            exps[pf_cnt]++;
        }
        pf_cnt++;
    }
    for (idx_t d = (idx_t)3; d <= temp / d; d += (idx_t)2)
    {
        if (temp % d == (uint64_t)0)
        {
            primes[pf_cnt] = (uint64_t)d;
            exps[pf_cnt]   = (idx_t)0;
            while (temp % d == (uint64_t)0)
            {
                temp /= d;
                exps[pf_cnt]++;
            }
            pf_cnt++;
        }
    }
    if (temp > (uint64_t)1)
    {
        primes[pf_cnt] = temp;
        exps[pf_cnt]   = (idx_t)1;
        pf_cnt++;
    }

    /* Determine maximal exponent among prime factors */
    idx_t max_e = (idx_t)1;
    for (i = 0; i < pf_cnt; ++i)
        if (exps[i] > max_e)
            max_e = exps[i];

    /* We only need to test k from 2 up to max_e (inclusive). */
    idx_t limit = max_e;
    if (limit < (idx_t)2)
        limit = (idx_t)2;

    seq_t best = (seq_t)0;
    bool   first = true;

    for (idx_t k = (idx_t)2; k <= limit; ++k)
    {
        seq_t m = (seq_t)1;
        for (i = 0; i < pf_cnt; ++i)
        {
            idx_t rem = exps[i] % k;
            if (rem == (idx_t)0)
                continue;
            idx_t need = k - rem;
            seq_t term = power_uint((seq_t)primes[i], need);
            m *= term;
        }
        if (first || m < best)
        {
            best = m;
            first = false;
        }
    }

    return best;
}
