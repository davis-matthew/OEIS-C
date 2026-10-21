/*
 * A062369  Dirichlet convolution of n and tau^2(n).
 * https://oeis.org/A062369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} d * tau(n/d)^2 (tau = number of divisors)
 * Range:     u8 n <= 23, u16 n <= 1439, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Recursive helper to enumerate all divisors of n and accumulate the result. */
static void dfs(const uint64_t *pr, const int *exp, size_t cnt,
                size_t idx, seq_t curD, seq_t curTauM, seq_t *sum)
{
    if (idx == cnt) {
        /* curD is the divisor d, curTauM is tau(n/d). */
        *sum += curD * curTauM * curTauM;
        return;
    }

    seq_t power = (seq_t)1;
    for (int k = 0; k <= exp[idx]; ++k) {
        seq_t newD = curD * power;
        seq_t newTauM = curTauM * (seq_t)(exp[idx] - k + 1);
        dfs(pr, exp, cnt, idx + 1, newD, newTauM, sum);
        power *= (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    /* Factor n into primes. */
    uint64_t pr[64];
    int exp[64];
    size_t cnt = 0;
    uint64_t m = (uint64_t)n;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            pr[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        pr[cnt] = m;
        exp[cnt] = 1;
        ++cnt;
    }

    seq_t sum = (seq_t)0;
    dfs(pr, exp, cnt, 0, (seq_t)1, (seq_t)1, &sum);
    return sum;
}
