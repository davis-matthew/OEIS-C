/*
 * A059361  a(1)=1; a(n) = A059333(a(n-1)) + a(n-1).
 * https://oeis.org/A059361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(1) = 1; a(n) = A059333(a(n-1)) + a(n-1), where A059333(x) =
 *            least m >= 1 with Omega(x) + Omega(m) = Omega(x + m). Each
 *            A059333 value is found by a search: a direct scan when Omega(x)
 *            is small, otherwise by listing every k = x + m in growing
 *            windows that has at least Omega(x)+1 prime factors (th...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A059361_PL (1u << 20)

static uint32_t *A059361_pr = NULL;
static size_t A059361_pc = 0;

static void A059361_init(void)
{
    if (A059361_pr) return;
    char *c = (char *)calloc(A059361_PL + 1, 1);
    A059361_pr = (uint32_t *)malloc(sizeof(uint32_t) * 90000);
    for (uint32_t i = 2; i <= A059361_PL; i++) {
        if (!c[i]) {
            A059361_pr[A059361_pc++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A059361_PL; j += i) c[j] = 1;
        }
    }
    free(c);
}

static unsigned A059361_om(uint64_t x)
{
    unsigned c = 0;
    for (size_t k = 0; k < A059361_pc; k++) {
        uint64_t p = A059361_pr[k];
        if (p * p > x) break;
        while (x % p == 0) { x /= p; c++; }
    }
    if (x > 1) c++;
    return c;
}

/* candidate list */
static uint64_t *A059361_cand = NULL;
static size_t A059361_cn = 0, A059361_ccap = 0;

static void A059361_push(uint64_t v)
{
    if (A059361_cn == A059361_ccap) {
        A059361_ccap = A059361_ccap ? 2 * A059361_ccap : 4096;
        A059361_cand = (uint64_t *)realloc(A059361_cand, sizeof(uint64_t) * A059361_ccap);
    }
    A059361_cand[A059361_cn++] = v;
}

/* is the least prime factor of j (j >= 2) at least pm? */
static bool A059361_lpf_ge(uint64_t j, uint64_t pm)
{
    for (size_t k = 0; k < A059361_pc; k++) {
        uint64_t q = A059361_pr[k];
        if (q >= pm) return true;
        if (q * q > j) return j >= pm;
        if (j % q == 0) return false;
    }
    return true;
}

/* enumerate all k in [lo,hi] with Omega(k) >= T: k = P * j, P = product of the
   T-1 smallest prime factors (nondecreasing), j >= 2 with lpf(j) >= largest of P */
static void A059361_dfs(uint64_t prod, size_t idx, unsigned cnt, uint64_t pm,
                        unsigned T, uint64_t lo, uint64_t hi)
{
    if (cnt == T - 1) {
        uint64_t jlo = (lo + prod - 1) / prod, jhi = hi / prod;
        if (jlo < 2) jlo = 2;
        for (uint64_t j = jlo; j <= jhi; j++)
            if (pm <= 2 || A059361_lpf_ge(j, pm)) A059361_push(prod * j);
        return;
    }
    unsigned rem = T - cnt; /* factors still to come, each >= p */
    for (size_t i = idx; i < A059361_pc; i++) {
        uint64_t p = A059361_pr[i];
        uint64_t v = prod;
        bool ok = true;
        for (unsigned r = 0; r < rem; r++) {
            if (v > hi / p) { ok = false; break; }
            v *= p;
        }
        if (!ok) break;
        A059361_dfs(prod * p, i, cnt + 1, p, T, lo, hi);
    }
}

static int A059361_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t A059361_A059333(uint64_t n)
{
    unsigned t = A059361_om(n);
    if (t <= 5) {
        for (uint64_t m = 1;; m++)
            if (t + A059361_om(m) == A059361_om(n + m)) return m;
    }
    if (A059361_om(n + 1) == t) return 1;
    unsigned T = t + 1;
    uint64_t lo = n + 2, W = 1u << 12;
    for (;;) {
        uint64_t hi = lo + W - 1;
        A059361_cn = 0;
        A059361_dfs(1, 0, 0, 2, T, lo, hi);
        qsort(A059361_cand, A059361_cn, sizeof(uint64_t), A059361_cmp);
        for (size_t i = 0; i < A059361_cn; i++) {
            uint64_t k = A059361_cand[i];
            if (A059361_om(k) == t + A059361_om(k - n)) return k - n;
        }
        lo = hi + 1;
        if (W < ((uint64_t)1 << 24)) W <<= 1;
    }
}

seq_t a(idx_t n)
{
    static uint64_t terms[128];
    static unsigned have = 0;
    A059361_init();
    if (have == 0) { terms[0] = 1; have = 1; }
    if (n < 1) return (seq_t)0;
    while (have < n && have < 128) {
        uint64_t p = terms[have - 1];
        terms[have] = p + A059361_A059333(p);
        have++;
    }
    return (seq_t)terms[n - 1];
}
