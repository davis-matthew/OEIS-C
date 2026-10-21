/*
 * A062370  a(n) = Sum_{i|n,j|n} sigma(i)*sigma(j)/sigma(gcd(i,j)), where
 *          sigma(n) = sum of divisors of n.
 * https://oeis.org/A062370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Multiplicative with a(p^e) = 1 + Σ_{k=1..e} (2k+1)·σ(p^k)
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (σ denotes the sum-of-divisors function)                               */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 but guard */
    uint64_t nn = (uint64_t)n;     /* use native type for factoring */

    /* small stack arrays for prime factors – enough for n < 2^64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    /* trial division factorization (fast enough for n <= ~10^6) */
    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]  = e;
            ++cnt;
        }
    }
    if (nn > 1) {                   /* remaining prime factor */
        primes[cnt] = nn;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* multiplicative accumulation */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* compute a(p^e) */
        seq_t ap = 1;                  /* starting with 1 */
        seq_t sigma = 1;               /* σ(p^0) = 1 */
        seq_t power = 1;               /* p^0 = 1 */

        for (uint64_t k = 1; k <= e; ++k) {
            power = power * (seq_t)p;   /* p^k */
            sigma = sigma + power;      /* σ(p^k) */

            /* term = (2k+1) * σ(p^k) */
            seq_t term = (seq_t)(2 * k + 1) * sigma;
            ap = ap + term;
        }
        result = result * ap;
    }
    return result;
}
