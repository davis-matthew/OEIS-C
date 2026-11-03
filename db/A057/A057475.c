/*
 * A057475  Number of k, 1 <= k <= n, such that gcd(n,k) = gcd(n+1,k) = 1.
 * https://oeis.org/A057475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k | 1 ≤ k ≤ n , gcd(k, n(n+1)) = 1 }
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static void factor_distinct(uint64_t n, uint64_t *pr, size_t *cnt)
{
    *cnt = 0;
    if (n <= 1) return;
    uint64_t x = n;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            pr[(*cnt)++] = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) pr[(*cnt)++] = x;
}

static int64_t rec_inclusion(uint64_t n, size_t start,
                             unsigned __int128 prod, int sign,
                             const uint64_t *pr, size_t cnt)
{
    int64_t res = 0;
    for (size_t j = start; j < cnt; ++j)
    {
        unsigned __int128 np = prod * pr[j];
        if (np > n) continue;
        uint64_t div = n / (uint64_t)np;
        res += sign * (int64_t)div;
        res += rec_inclusion(n, j + 1, np, -sign, pr, cnt);
    }
    return res;
}

static seq_t inclusion_exclusion(uint64_t n, const uint64_t *pr, size_t cnt)
{
    int64_t res = n + rec_inclusion(n, 0, 1, -1, pr, cnt);
    if (res < 0) res = 0;          /* safety, not expected */
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    uint64_t primes[32];
    size_t cnt = 0;
    factor_distinct((uint64_t)n, primes, &cnt);

    uint64_t tmppr[32];
    size_t cnt2 = 0;
    uint64_t temp_n1 = (uint64_t)(n + 1);
    factor_distinct(temp_n1, tmppr, &cnt2);

    for (size_t i = 0; i < cnt2; ++i) primes[cnt++] = tmppr[i];

    return inclusion_exclusion((uint64_t)n, primes, cnt);
}
