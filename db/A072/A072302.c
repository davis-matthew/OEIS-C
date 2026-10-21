/*
 * A072302  Number of positive integers not exceeding n that are relatively
 *          prime to phi(n).
 * https://oeis.org/A072302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|phi(n)} mu(d) * floor(n/d) (mobius mu)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/phi.h"

static void dfs_div(const uint64_t *primes, const uint64_t *exps, size_t k,
                    size_t idx, uint64_t cur, int mu, idx_t n,
                    seq_t *pos, seq_t *neg)
{
    if (idx == k) {
        if (mu == 0) return;                 /* mu(d)=0, skip */
        seq_t q = (seq_t)(n / cur);          /* floor(n/d)  */
        if (mu == 1) *pos += q;
        else          *neg += q;             /* mu==-1  */
        return;
    }
    /* exp = 0 */
    dfs_div(primes, exps, k, idx + 1, cur, mu, n, pos, neg);

    uint64_t p = primes[idx];
    int e = (int)exps[idx];
    uint64_t cur_mul = cur;
    for (int exp = 1; exp <= e; ++exp) {
        cur_mul *= p;
        int mu_new = (exp > 1) ? 0 : -mu;   /* toggle sign if exp==1 */
        dfs_div(primes, exps, k, idx + 1, cur_mul, mu_new, n, pos, neg);
    }
}

seq_t a(idx_t n)
{
    /* Compute phi(n) and cast to 64‑bit unsigned */
    seq_t phi_val = phi(n);
    uint64_t phi_n  = (uint64_t)phi_val;

    /* Factor phi_n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(phi_n, primes, exps, &cnt);

    seq_t pos = 0, neg = 0;
    dfs_div(primes, exps, cnt, 0, 1ULL, 1, n, &pos, &neg);

    /* pos >= neg, so subtraction is safe in unsigned arithmetic */
    return pos - neg;
}
