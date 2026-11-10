/*
 * A065373  Number of iterations of A065371 starting at n until 1 is reached.
 * https://oeis.org/A065373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = 1 + a(A065371(n)), where A065371 is
 *            multiplicative with A065371(prime(k)) = prime(k) - k. Computed
 *            by direct iteration until 1 is reached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9999 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint32_t *A065373_pl = NULL;
static size_t A065373_cnt = 0;
static uint64_t A065373_lim = 1;

static void A065373_ensure(uint64_t lim)
{
    if (A065373_lim >= lim) return;
    if (lim < 2 * A065373_lim) lim = 2 * A065373_lim;
    if (lim < 1000) lim = 1000;
    bool *s = (bool *)malloc((size_t)(lim + 1) * sizeof(bool));
    if (!s) abort();
    memset(s, 1, (size_t)(lim + 1));
    s[0] = false; s[1] = false;
    for (uint64_t i = 2; i * i <= lim; ++i)
        if (s[i])
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = false;
    size_t c = 0;
    for (uint64_t i = 2; i <= lim; ++i) if (s[i]) ++c;
    uint32_t *nl = (uint32_t *)realloc(A065373_pl, c * sizeof(uint32_t));
    if (!nl) abort();
    A065373_pl = nl;
    A065373_cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) if (s[i]) A065373_pl[A065373_cnt++] = (uint32_t)i;
    A065373_lim = lim;
    free(s);
}

static uint64_t A065373_index(uint64_t p)
{
    size_t lo = 0, hi = A065373_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if ((uint64_t)A065373_pl[mid] >= p) hi = mid; else lo = mid + 1;
    }
    return (uint64_t)lo + 1;
}

static uint64_t A065373_step(uint64_t m)
{
    uint64_t res = 1;
    for (size_t i = 0; i < A065373_cnt; ++i) {
        uint64_t p = A065373_pl[i];
        if (p * p > m) break;
        if (m % p == 0) {
            uint64_t base = p - (uint64_t)(i + 1);
            while (m % p == 0) { m /= p; res *= base; }
        }
    }
    if (m > 1) res *= m - A065373_index(m);
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    uint64_t m = (uint64_t)n;
    A065373_ensure(m);
    unsigned long long cnt = 0;
    while (m > 1) {
        m = A065373_step(m);
        ++cnt;
    }
    return (seq_t)cnt;
}
