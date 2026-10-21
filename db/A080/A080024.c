/*
 * A080024  Number of divisors d of n such that in binary representation d and
 *          n/d have the same number of 1's as n.
 * https://oeis.org/A080024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Compute a(n): number of divisors d of n such that
   popcount(d) = popcount(n/d) = popcount(n) */
static void rec_div(size_t idx,
                    idx_t cur,
                    const idx_t *primes,
                    const idx_t *exps,
                    size_t factor_cnt,
                    idx_t n,
                    idx_t pop_n,
                    unsigned long long *cnt)
{
    if (idx == factor_cnt) {
        idx_t other = n / cur;
        unsigned int pop_cur   = __builtin_popcountll((unsigned long long)cur);
        unsigned int pop_other = __builtin_popcountll((unsigned long long)other);
        if (pop_cur == pop_n && pop_other == pop_n)
            (*cnt)++;
        return;
    }
    idx_t p = primes[idx];
    idx_t val = 1;
    for (idx_t e = 0; e <= exps[idx]; ++e) {
        rec_div(idx + 1, cur * val, primes, exps, factor_cnt,
                n, pop_n, cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* defined only for n>=1 */

    /* popcount of n */
    idx_t pop_n = __builtin_popcountll((unsigned long long)n);

    /* factorisation of n into primes and exponents */
    idx_t primes[64];
    idx_t exps[64];
    size_t factor_cnt = 0;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[factor_cnt] = p;
            exps[factor_cnt]  = e;
            factor_cnt++;
        }
    }
    if (temp > 1) {
        primes[factor_cnt] = temp;
        exps[factor_cnt]  = 1;
        factor_cnt++;
    }

    /* enumerate divisors and count those satisfying the condition */
    unsigned long long count = 0;
    rec_div(0, (idx_t)1, primes, exps, factor_cnt,
            n, pop_n, &count);

    return (seq_t)count;
}
