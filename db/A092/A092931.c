/*
 * A092931  Number of ways of factorizing n into parts whose sum divides n.
 * https://oeis.org/A092931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways of factorizing n into parts (>1) whose sum
 *            divides n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *spf = NULL;
static idx_t spf_limit = 0;

static void ensure_spf(idx_t n)
{
    if (n <= spf_limit) return;
    free(spf);
    size_t sz = (size_t)n + 1;
    spf = (uint32_t*)malloc(sz * sizeof(uint32_t));
    if (!spf) exit(1);
    for (size_t i = 0; i < sz; ++i) spf[i] = (uint32_t)i;
    for (idx_t i = 2; i * i <= n; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
        }
    }
    spf_limit = n;
}

static void enumerate_divisors(idx_t n, idx_t min_f, idx_t *divs, size_t *cnt)
{
    uint32_t primes[16];
    int exps[16];
    int pcnt = 0;
    idx_t tmp = n;
    while (tmp > 1) {
        uint32_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[pcnt] = p;
        exps[pcnt] = e;
        ++pcnt;
    }
    divs[0] = 1;
    size_t curcnt = 1;
    for (int i = 0; i < pcnt; ++i) {
        size_t oldcnt = curcnt;
        uint64_t mult = 1;
        for (int e = 1; e <= exps[i]; ++e) {
            mult *= (uint64_t)primes[i];
            for (size_t j = 0; j < oldcnt; ++j) {
                divs[curcnt++] = divs[j] * mult;
            }
        }
    }
    size_t out = 0;
    for (size_t i = 0; i < curcnt; ++i) {
        idx_t d = divs[i];
        if (d >= min_f && d > 1) {
            divs[out++] = d;
        }
    }
    *cnt = out;
}

static void dfs(idx_t cur, idx_t min_f, idx_t sum, seq_t *cnt, idx_t orig_n)
{
    if (cur == 1) {
        if (orig_n % sum == 0) (*cnt)++;
        return;
    }
    idx_t divs[2048];
    size_t divcnt = 0;
    enumerate_divisors(cur, min_f, divs, &divcnt);
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t f = divs[i];
        dfs(cur / f, f, sum + f, cnt, orig_n);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    ensure_spf(n);
    seq_t res = 0;
    dfs(n, 2, 0, &res, n);
    return res;
}
