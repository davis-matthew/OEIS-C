/*
 * A096825  Maximal size of an antichain in divisor lattice D(n).
 * https://oeis.org/A096825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^k in ∏p (1 + x + … + x^k_p), where k =
 *            ⌊(k₁ + … + k_q)/2⌋ and n = ∏p p^k_p (prime factorisation). The
 *            implementation factors n by trial division and counts the number
 *            of divisor‑exponent combinations with total exponent sum k using
 *            dynamic programming
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void factor_uint64(uint64_t n, uint64_t *primes, uint64_t *exps,
                          size_t *cnt)
{
    size_t c = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            primes[c] = d;
            exps[c]   = e;
            ++c;
        }
        d += (d == 2) ? 1 : 2;   /* 2, 3, 5, 7, … */
    }
    if (n > 1) {                 /* remaining prime factor */
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    uint64_t val = (uint64_t)n;
    /* 64-bit factorisation: allow up to ~1e12 with small overhead. */
    uint64_t primes[64], exps[64];
    size_t prime_cnt = 0;
    factor_uint64(val, primes, exps, &prime_cnt);

    /* Total number of prime factors (with multiplicity). */
    idx_t omega = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
        omega += (idx_t)exps[i];

    idx_t k = omega >> 1;   /* floor(omega/2) */

    /* DP array: dp[s] = #ways to obtain exponent‑sum s from processed primes. */
    seq_t *dp = (seq_t*)malloc((k + 1) * sizeof(seq_t));
    if (!dp)
        return 0;          /* unlikely for n ≤ 10⁴, but graceful fallback */

    for (idx_t s = 0; s <= k; ++s)
        dp[s] = 0;
    dp[0] = 1;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t maxe = exps[pi];
        for (long long s = (long long)k; s >= 0; --s) {
            seq_t cur = dp[(idx_t)s];
            if (!cur) continue;
            for (uint64_t e = 1; e <= maxe; ++e) {
                idx_t ns = (idx_t)s + e;
                if (ns > k) break;
                dp[ns] = dp[ns] + cur;
            }
        }
    }

    seq_t ans = dp[k];
    free(dp);
    return ans;
}
