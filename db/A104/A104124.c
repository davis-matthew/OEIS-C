/*
 * A104124  a(n) = number of ways to write n = (2m-1)^2 * k, m >= k, k and m =
 *          positive integers.
 * https://oeis.org/A104124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts representations of n as (2m-1)^2 * k with m ≥ k. For
 *            each odd divisor O such that O² divides n, let k = n / O². Count
 *            O when k ≤ (O+1)/2
 * Range:     u8 n <= 8729, u16 n <= 9887, u32 n <= 9887, u64 n <= 9887,
 *            u128 n <= 9887, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void dfs(idx_t n, int idx, int maxf,
                const idx_t primes[], const int exps[],
                seq_t curO, seq_t *cnt)
{
    if (idx == maxf) {
        seq_t O  = curO;
        seq_t O2 = O * O;
        seq_t k  = (seq_t)n / O2;
        if (k <= (O + 1) / 2)
            *cnt += 1;
        return;
    }
    idx_t p   = primes[idx];
    int   e   = exps[idx];
    int   max_f = e / 2;
    seq_t prod = 1;
    for (int f = 0; f <= max_f; ++f) {
        dfs(n, idx + 1, maxf, primes, exps, curO * prod, cnt);
        prod *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* factorize n, ignoring the power of 2 for odd-divisor enumeration */
    idx_t tmp   = n;
    idx_t primes[64];
    int  exps[64];
    int  maxf = 0;

    /* remove factors of 2 */
    while ((tmp & 1) == 0)
        tmp >>= 1;

    /* trial division by odd numbers */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[maxf] = p;
            exps[maxf]   = e;
            ++maxf;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[maxf] = tmp;
        exps[maxf]   = 1;
        ++maxf;
    }

    seq_t count = 0;
    dfs(n, 0, maxf, primes, exps, 1, &count);

    return count;
}
