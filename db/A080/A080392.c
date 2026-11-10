/*
 * A080392  Numbers k such that A000984(k) mod k = 0 and A080383(k) != 7.
 * https://oeis.org/A080392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search over even m: keep m if m | A000984-type central binomial
 *            C(m,m/2) and A080383(m) != 7 (count of j with C(m,j) |
 *            C(m,m/2)), pruned by the largest prime p <= m/2 (only j with
 *            min(j,m-j) <= m-2p or |j-m/2| <= (m-2p)/2 can work)
 * Range:     u8 n <= 1, u16 n <= 274, u32 n <= 274, u64 n <= 274,
 *            u128 n <= 274, big any n
 * Validated: standard, pass, 61 terms through n = 249 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint32_t *A080392_pr = NULL;
static size_t A080392_np = 0;
static uint64_t A080392_plim = 0;

static uint64_t *A080392_terms = NULL;
static size_t A080392_nt = 0, A080392_cap = 0;
static uint64_t A080392_next = 2;

static void A080392_ensure(uint64_t lim)
{
    if (lim <= A080392_plim) return;
    if (lim < 2 * A080392_plim) lim = 2 * A080392_plim;
    if (lim < 1000) lim = 1000;
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    free(A080392_pr);
    A080392_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
    A080392_np = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!c[i]) {
            A080392_pr[A080392_np++] = (uint32_t)i;
            for (uint64_t q = i * i; q <= lim; q += i) c[q] = 1;
        }
    }
    free(c);
    A080392_plim = lim;
}

static uint64_t A080392_leg(uint64_t x, uint64_t p)
{
    uint64_t s = 0;
    while (x) { x /= p; s += x; }
    return s;
}

static uint64_t A080392_vbin(uint64_t m, uint64_t j, uint64_t p)
{
    return A080392_leg(m, p) - A080392_leg(j, p) - A080392_leg(m - j, p);
}

/* does m divide binomial(m, m/2) ? */
static int A080392_divides(uint64_t m)
{
    uint64_t k = m / 2, r = m;
    for (uint64_t p = 2; p * p <= r; p++) {
        if (r % p == 0) {
            uint64_t e = 0;
            while (r % p == 0) { r /= p; e++; }
            if (A080392_vbin(m, k, p) < e) return 0;
        }
    }
    if (r > 1) {
        if (A080392_vbin(m, k, r) < 1) return 0;
    }
    return 1;
}

/* is binomial(m,j) | binomial(m,m/2) ? */
static int A080392_dividesj(uint64_t m, uint64_t j)
{
    uint64_t k = m / 2;
    for (size_t i = 0; i < A080392_np && A080392_pr[i] <= m; i++) {
        uint64_t p = A080392_pr[i];
        if (A080392_vbin(m, j, p) > A080392_vbin(m, k, p)) return 0;
    }
    return 1;
}

static uint64_t A080392_count(uint64_t m)
{
    int64_t M = (int64_t)m, K = M / 2;
    /* largest prime <= m/2 */
    size_t lo = 0, hi = A080392_np;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if ((int64_t)A080392_pr[mid] <= K) lo = mid + 1; else hi = mid;
    }
    int64_t r = (lo == 0) ? M : M - 2 * (int64_t)A080392_pr[lo - 1];
    if (r > M) r = M;
    uint64_t cnt = 0;
    int64_t j;
    /* low range */
    for (j = 0; j <= r && j <= M; j++)
        if (A080392_dividesj(m, (uint64_t)j)) cnt++;
    /* high range */
    {
        int64_t s = M - r; if (s < r + 1) s = r + 1;
        for (j = s; j <= M; j++)
            if (A080392_dividesj(m, (uint64_t)j)) cnt++;
    }
    /* middle range */
    {
        int64_t s = K - r / 2, e = K + r / 2;
        if (s < r + 1) s = r + 1;
        if (e > M - r - 1) e = M - r - 1;
        for (j = s; j <= e; j++)
            if (A080392_dividesj(m, (uint64_t)j)) cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    while (A080392_nt < (size_t)n) {
        uint64_t m = A080392_next;
        A080392_next += 2;
        if (!A080392_divides(m)) continue;
        A080392_ensure(m);
        if (A080392_count(m) == 7) continue;
        if (A080392_nt == A080392_cap) {
            A080392_cap = A080392_cap ? A080392_cap * 2 : 64;
            A080392_terms = (uint64_t *)realloc(A080392_terms, A080392_cap * sizeof(uint64_t));
        }
        A080392_terms[A080392_nt++] = m;
    }
    return (seq_t)A080392_terms[(size_t)n - 1];
}
