/*
 * A131576  Number of ways to represent n as a sum of an even number of
 *          consecutive integers.
 * https://oeis.org/A131576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of odd divisors d of n with d*d > 2*n That equals
 *            the number of ways to represent n as a sum of an even number of
 *            consecutive integers
 * Range:     u8 n <= 493, u16 n <= 9198, u32 n <= 9198, u64 n <= 9198,
 *            u128 n <= 9198, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t dfs(const uint64_t *primes, const int *exps, int idx, int cnt,
                 seq_t val, seq_t twice_n)
{
    if (idx == cnt) {
        return (val * val > twice_n) ? (seq_t)1 : (seq_t)0;
    }
    seq_t sum = 0;
    seq_t v = (seq_t)1;
    for (int e = 0; e <= exps[idx]; ++e) {
        sum += dfs(primes, exps, idx + 1, cnt, val * v, twice_n);
        v *= (seq_t)primes[idx];
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* remove all factors of 2 – only odd divisors matter */
    idx_t temp = n;
    while ((temp & 1) == 0) temp >>= 1;

    /* factor the remaining odd part */
    uint64_t primes[64];
    int exps[64];
    int pcnt = 0;

    idx_t m = temp;
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            primes[pcnt] = (uint64_t)p;
            exps[pcnt] = e;
            ++pcnt;
        }
    }
    if (m > 1) {
        primes[pcnt] = (uint64_t)m;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* threshold = 2*n, computed as seq_t to avoid overflow */
    seq_t twice_n = (seq_t)2 * (seq_t)n;

    return dfs(primes, exps, 0, pcnt, (seq_t)1, twice_n);
}
