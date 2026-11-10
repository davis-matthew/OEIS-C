/*
 * A208815  n for which A079277(n) + phi(n) < n.
 * https://oeis.org/A208815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: n is a term iff no k in [n - phi(n), n-1] has all its
 *            prime factors dividing n, i.e. A079277(n) + phi(n) < n. Uses a
 *            segmented sieve for factorizations
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 1751, u64 n <= 1751,
 *            u128 n <= 1751, big any n
 * Validated: standard, pass, 56 terms through n = 1683 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A208815_SEG 32768

static uint32_t *A208815_bp = 0;
static size_t A208815_nbp = 0;
static uint64_t *A208815_terms = 0;
static size_t A208815_cnt = 0, A208815_cap = 0;
static uint64_t A208815_lo = 2;

static uint8_t A208815_scnt[A208815_SEG];
static uint32_t A208815_spl[A208815_SEG][12];
static uint64_t A208815_sfull[A208815_SEG];

static uint64_t A208815_pr[16];
static uint64_t A208815_m, A208815_c;

static void A208815_init(void)
{
    const uint32_t L = 65536;
    char *s = (char *)calloc(L + 1, 1);
    size_t cap = 8000;
    A208815_bp = (uint32_t *)malloc(cap * sizeof(uint32_t));
    for (uint32_t i = 2; i <= L; i++) {
        if (!s[i]) {
            A208815_bp[A208815_nbp++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= L; j += i) s[j] = 1;
        }
    }
    free(s);
}

/* exists k = cur * (product of powers of pr[0..i]) with c <= k < m ? */
static bool A208815_dfs(int i, uint64_t cur)
{
    if (i == 0) {
        uint64_t p = A208815_pr[0], v = cur;
        while (v * p < A208815_m) v *= p;
        return v >= A208815_c;
    }
    uint64_t v = cur;
    while (v < A208815_m) {
        if (A208815_dfs(i - 1, v)) return true;
        v *= A208815_pr[i];
    }
    return false;
}

static void A208815_push(uint64_t m)
{
    if (A208815_cnt == A208815_cap) {
        A208815_cap = A208815_cap ? A208815_cap * 2 : 1024;
        A208815_terms = (uint64_t *)realloc(A208815_terms, A208815_cap * sizeof(uint64_t));
    }
    A208815_terms[A208815_cnt++] = m;
}

static void A208815_segment(void)
{
    uint64_t lo = A208815_lo, hi = lo + A208815_SEG - 1;
    for (size_t i = 0; i < A208815_SEG; i++) {
        A208815_scnt[i] = 0;
        A208815_sfull[i] = 1;
    }
    for (size_t k = 0; k < A208815_nbp; k++) {
        uint64_t p = A208815_bp[k];
        if (p * p > hi) break;
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t j = st; j <= hi; j += p) {
            size_t id = (size_t)(j - lo);
            A208815_spl[id][A208815_scnt[id]++] = (uint32_t)p;
            A208815_sfull[id] *= p;
        }
        uint64_t pk = p * p;
        while (pk <= hi) {
            st = ((lo + pk - 1) / pk) * pk;
            for (uint64_t j = st; j <= hi; j += pk)
                A208815_sfull[(size_t)(j - lo)] *= p;
            pk *= p;
        }
    }
    for (size_t id = 0; id < A208815_SEG; id++) {
        uint64_t m = lo + id;
        int t = 0;
        for (int q = 0; q < A208815_scnt[id]; q++)
            A208815_pr[t++] = A208815_spl[id][q];
        uint64_t cof = m / A208815_sfull[id];
        if (cof > 1) A208815_pr[t++] = cof;
        if (t < 2) continue; /* primes and prime powers are never terms */
        uint64_t phi = m;
        for (int q = 0; q < t; q++) phi = phi / A208815_pr[q] * (A208815_pr[q] - 1);
        A208815_m = m;
        A208815_c = m - phi;
        if (!A208815_dfs(t - 1, 1)) A208815_push(m);
    }
    A208815_lo = hi + 1;
}

seq_t a(idx_t n)
{
    if (!A208815_bp) A208815_init();
    while (A208815_cnt < (size_t)n) A208815_segment();
    return (seq_t)A208815_terms[n - 1];
}
