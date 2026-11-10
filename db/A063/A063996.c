/*
 * A063996  Numbers k such that ud(k) = sopf(k)-1, where ud(k)=A034444(k) and
 *          sopf(k)=A008472(k).
 * https://oeis.org/A063996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search -- k is a term iff 2^omega(k) + 1 = sopf(k); enumerate
 *            sets of m distinct primes summing to 2^m+1, then all numbers
 *            with exactly that prime support up to a growing bound, sort, and
 *            pick the n-th
 * Range:     u8 n <= 20, u16 n <= 160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9344 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define A063996_PLIM ((1u << 17) + 8)

static uint8_t *A063996_isp = NULL;   /* isp[x] = 1 if x prime */
static uint32_t *A063996_pr = NULL;
static size_t A063996_npr = 0;

static uint64_t *A063996_terms = NULL;
static size_t A063996_cnt = 0, A063996_cap = 0;
static uint64_t A063996_B = 0;

static uint64_t A063996_set[32];

static void A063996_sieve(void)
{
    if (A063996_isp) return;
    A063996_isp = (uint8_t *)malloc(A063996_PLIM);
    for (size_t i = 0; i < A063996_PLIM; ++i) A063996_isp[i] = 1;
    A063996_isp[0] = A063996_isp[1] = 0;
    for (size_t i = 2; i * i < A063996_PLIM; ++i)
        if (A063996_isp[i])
            for (size_t j = i * i; j < A063996_PLIM; j += i) A063996_isp[j] = 0;
    size_t c = 0;
    for (size_t i = 2; i < A063996_PLIM; ++i) if (A063996_isp[i]) ++c;
    A063996_pr = (uint32_t *)malloc(sizeof(uint32_t) * c);
    c = 0;
    for (size_t i = 2; i < A063996_PLIM; ++i) if (A063996_isp[i]) A063996_pr[c++] = (uint32_t)i;
    A063996_npr = c;
}

static void A063996_push(uint64_t v)
{
    if (A063996_cnt == A063996_cap) {
        A063996_cap = A063996_cap ? A063996_cap * 2 : 1024;
        A063996_terms = (uint64_t *)realloc(A063996_terms, sizeof(uint64_t) * A063996_cap);
    }
    A063996_terms[A063996_cnt++] = v;
}

/* all numbers with support exactly A063996_set[0..m-1], value <= B */
static void A063996_expo(int i, int m, uint64_t val)
{
    if (i == m) { A063996_push(val); return; }
    uint64_t q = A063996_set[i];
    for (;;) {
        A063996_expo(i + 1, m, val);
        if (val > A063996_B / q) break;
        val *= q;
    }
}

/* choose k more primes, each > minp, summing to r, product with prod <= B */
static void A063996_sets(int depth, int m, int k, uint64_t r, uint64_t minp,
                         size_t start, uint64_t prod)
{
    if (k == 1) {
        if (r > minp && r < A063996_PLIM && A063996_isp[r] && prod <= A063996_B / r) {
            A063996_set[depth] = r;
            A063996_expo(0, m, prod * r);
        }
        return;
    }
    for (size_t i = start; i < A063996_npr; ++i) {
        uint64_t p = A063996_pr[i];
        if (p * (uint64_t)k >= r) break;          /* remaining primes > p */
        /* prod * p^k <= B */
        uint64_t t = prod;
        bool ok = true;
        for (int j = 0; j < k; ++j) {
            if (t > A063996_B / p) { ok = false; break; }
            t *= p;
        }
        if (!ok) break;
        A063996_set[depth] = p;
        A063996_sets(depth + 1, m, k - 1, r - p, p, i + 1, prod * p);
    }
}

static int A063996_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A063996_build(uint64_t B)
{
    A063996_sieve();
    A063996_B = B;
    A063996_cnt = 0;
    uint64_t prim = 1;
    for (int m = 1; m <= 16; ++m) {
        uint64_t p = A063996_pr[m - 1];
        if (prim > B / p) break;
        prim *= p;
        uint64_t T = ((uint64_t)1 << m) + 1;
        A063996_sets(0, m, m, T, 1, 0, 1);
    }
    if (A063996_cnt)
        qsort(A063996_terms, A063996_cnt, sizeof(uint64_t), A063996_cmp);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A063996_B == 0) A063996_build((uint64_t)1 << 20);
    while (A063996_cnt < (size_t)n) {
        uint64_t nb = (A063996_B > UINT64_MAX / 8) ? UINT64_MAX : A063996_B * 8;
        if (nb == A063996_B) break;
        A063996_build(nb);
    }
    if (A063996_cnt < (size_t)n) return 0;
    return (seq_t)A063996_terms[n - 1];
}
