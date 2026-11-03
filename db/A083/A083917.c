/*
 * A083917  Number of divisors of n that are congruent to 7 modulo 10.
 * https://oeis.org/A083917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of n that are ≡ 7 (mod 10)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9932 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static void init_sieve(uint32_t **primes_out, size_t *cnt_out)
{
    const size_t LIMIT = 100000;               /* primes up to 10^5 are enough for n ≤ 10^4 */
    static int first = 1;
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;

    if (first) {
        first = 0;
        uint32_t *sieve = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
        for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = (uint32_t)i;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (sieve[i] == i) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    if (sieve[j] == j) sieve[j] = (uint32_t)i;
            }
        }
        for (size_t i = 2; i <= LIMIT; ++i)
            if (sieve[i] == i) {
                primes = (uint32_t*)realloc(primes, (primes_cnt + 1) * sizeof(uint32_t));
                primes[primes_cnt++] = (uint32_t)i;
            }
        free(sieve);
    }

    *primes_out = primes;
    *cnt_out = primes_cnt;
}

static void factor_uint64(uint64_t n, uint32_t *primes, size_t primes_cnt,
                          uint32_t *pf, int *pe, size_t *cnt)
{
    size_t idx = 0;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= n; ++i) {
        uint32_t p = primes[i];
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) { n /= p; ++e; }
            pf[idx] = p;
            pe[idx] = e;
            ++idx;
        }
    }
    if (n > 1) {
        pf[idx] = (uint32_t)n;
        pe[idx] = 1;
        ++idx;
    }
    *cnt = idx;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                  /* sequence offset is 1 */
    /* obtain primes list once */
    static uint32_t *primes = NULL;
    static size_t primes_cnt = 0;
    if (!primes) init_sieve(&primes, &primes_cnt);

    /* factor n */
    uint32_t pf[64];
    int pe[64];
    size_t nc = 0;
    factor_uint64((uint64_t)n, primes, primes_cnt, pf, pe, &nc);

    /* dynamic programming over residues modulo 10 */
    uint64_t cnt[10] = {0};
    cnt[1 % 10] = 1;                      /* divisor 1 */
    for (size_t i = 0; i < nc; ++i) {
        uint8_t remCnt[10] = {0};
        uint8_t rem = 1 % 10;
        for (int k = 0; k <= pe[i]; ++k) {
            remCnt[rem]++;
            rem = (rem * pf[i]) % 10;
        }
        uint64_t next[10] = {0};
        for (int r = 0; r < 10; ++r) if (cnt[r]) {
            for (int rr = 0; rr < 10; ++rr) if (remCnt[rr]) {
                next[(r * rr) % 10] += cnt[r] * remCnt[rr];
            }
        }
        memcpy(cnt, next, sizeof(cnt));
    }
    return (seq_t)cnt[7];
}
