/*
 * A059359  Numbers n such that n = A059333(A059333(n)).
 * https://oeis.org/A059359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search: k is a term iff m = A059333(k) has A059333(m) = k, where
 *            A059333(k) = least m >= 1 with bigomega(k) + bigomega(m) =
 *            bigomega(k+m). Because the condition is symmetric in (k, m),
 *            A059333(m) = k holds exactly when no m' < k satisfies it for m.
 *            bigomega comes from a sieve that grows as needed
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static unsigned char *A059359_om = NULL;
static uint64_t A059359_L = 0;
static const uint64_t A059359_CAP = (uint64_t)1 << 26;
static uint32_t *A059359_pr = NULL;
static size_t A059359_npr = 0;

static void A059359_build(uint64_t L)
{
    unsigned char *om = (unsigned char *)calloc((size_t)L, 1);
    uint64_t i, q, j;
    for (i = 2; i < L; ++i) {
        if (om[i] == 0) {
            for (q = i; q < L; q *= i) {
                for (j = q; j < L; j += q) om[j]++;
                if (q > L / i) break;
            }
        }
    }
    free(A059359_om);
    A059359_om = om;
    A059359_L = L;
    if (A059359_pr == NULL) {
        uint64_t lim = 65536;
        if (lim > L) lim = L;
        A059359_pr = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)lim);
        A059359_npr = 0;
        for (i = 2; i < lim; ++i)
            if (om[i] == 1) A059359_pr[A059359_npr++] = (uint32_t)i;
    }
}

static void A059359_init(void)
{
    if (A059359_om == NULL) A059359_build((uint64_t)1 << 17);
}

static int A059359_grow(void)
{
    if (A059359_L >= A059359_CAP) return 0;
    uint64_t L = A059359_L * 2;
    if (L > A059359_CAP) L = A059359_CAP;
    A059359_build(L);
    return 1;
}

static unsigned A059359_omega_slow(uint64_t x)
{
    unsigned cnt = 0;
    size_t i;
    uint64_t p;
    for (i = 0; i < A059359_npr; ++i) {
        p = A059359_pr[i];
        if (p * p > x) break;
        while (x % p == 0) { ++cnt; x /= p; }
    }
    if (i == A059359_npr) {
        for (p = 65537; p * p <= x; p += 2)
            while (x % p == 0) { ++cnt; x /= p; }
    }
    if (x > 1) ++cnt;
    return cnt;
}

static unsigned A059359_omega(uint64_t x)
{
    if (x < A059359_L) return A059359_om[x];
    return A059359_omega_slow(x);
}

/* A059333(k) */
static uint64_t A059359_f(uint64_t k)
{
    while (k + 2 >= A059359_L && A059359_grow()) { }
    unsigned ok = A059359_omega(k);
    uint64_t m = 1;
    for (;;) {
        if (k + m < A059359_L) {
            const unsigned char *om = A059359_om;
            uint64_t lim = A059359_L - k;
            for (; m < lim; ++m)
                if (ok + om[m] == om[k + m]) return m;
            if (A059359_grow()) continue;
        }
        if (ok + A059359_omega(m) == A059359_omega(k + m)) return m;
        ++m;
    }
}

/* true iff A059333(m) == k, given that (k, m) satisfies the condition */
static int A059359_back(uint64_t m, uint64_t k)
{
    unsigned omm = A059359_omega(m);
    uint64_t mp;
    for (mp = 1; mp < k; ++mp)
        if (omm + A059359_omega(mp) == A059359_omega(m + mp)) return 0;
    return 1;
}

static uint64_t *A059359_terms = NULL;
static size_t A059359_count = 0;
static size_t A059359_cap = 0;
static uint64_t A059359_next = 1;

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    A059359_init();
    while (A059359_count < want) {
        uint64_t k = A059359_next++;
        uint64_t m = A059359_f(k);
        if (A059359_back(m, k)) {
            if (A059359_count == A059359_cap) {
                A059359_cap = A059359_cap ? A059359_cap * 2 : 64;
                A059359_terms = (uint64_t *)realloc(A059359_terms, A059359_cap * sizeof(uint64_t));
            }
            A059359_terms[A059359_count++] = k;
        }
    }
    return (seq_t)A059359_terms[want - 1];
}
