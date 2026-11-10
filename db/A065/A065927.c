/*
 * A065927  Records in A065925.
 * https://oeis.org/A065927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th record value of
 *            A065925(m) = min{k >= 1 : sopf(m+k) = sopf(k)}, sopf = A008472.
 *            The values A065925(m) are computed for m = 1, 2, 3, ... in
 *            order. sopf(k) comes from a table for small k and from a
 *            segmented sieve beyond it
 * Range:     u8 n <= 4, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* ---------- prime list for the segmented sieve ---------- */
static uint32_t *A065927_pr = NULL;
static size_t A065927_npr = 0;
static uint64_t A065927_plim = 0;

static void A065927_primes(uint64_t hi) {
    if (A065927_plim && A065927_plim * A065927_plim > hi) return;
    uint64_t P = A065927_plim ? A065927_plim : 1024;
    while (P * P <= hi) P *= 2;
    unsigned char *c = (unsigned char *)malloc((size_t)P + 1);
    if (!c) return;
    memset(c, 0, (size_t)P + 1);
    free(A065927_pr);
    A065927_pr = (uint32_t *)malloc(((size_t)P / 2 + 16) * sizeof(uint32_t));
    A065927_npr = 0;
    for (uint64_t i = 2; i <= P; i++) {
        if (!c[i]) {
            A065927_pr[A065927_npr++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= P; j += i) c[j] = 1;
        }
    }
    free(c);
    A065927_plim = P;
}

/* sopf(x) for x in [lo, lo+len), lo >= 1; prod is scratch space */
static void A065927_seg(uint64_t lo, size_t len, uint64_t *sum, uint64_t *prod) {
    uint64_t hi = lo + (uint64_t)len - 1;
    A065927_primes(hi);
    for (size_t i = 0; i < len; i++) { sum[i] = 0; prod[i] = 1; }
    for (size_t t = 0; t < A065927_npr; t++) {
        uint64_t p = A065927_pr[t];
        if (p * p > hi) break;
        uint64_t first = ((lo + p - 1) / p) * p;
        for (uint64_t j = first - lo; j < len; j += p) { sum[j] += p; prod[j] *= p; }
        uint64_t pe = p * p;
        for (;;) {
            first = ((lo + pe - 1) / pe) * pe;
            for (uint64_t j = first - lo; j < len; j += pe) prod[j] *= p;
            if (pe > hi / p) break;
            pe *= p;
        }
    }
    for (size_t i = 0; i < len; i++) {
        uint64_t c = (lo + i) / prod[i];
        if (c > 1) sum[i] += c;
    }
}

/* ---------- small sopf table ---------- */
#define A065927_L ((uint64_t)1 << 22)
static uint32_t *A065927_T = NULL;

static void A065927_table(void) {
    if (A065927_T) return;
    size_t CH = (size_t)1 << 16;
    uint64_t *s = (uint64_t *)malloc(CH * sizeof(uint64_t));
    uint64_t *p = (uint64_t *)malloc(CH * sizeof(uint64_t));
    A065927_T = (uint32_t *)malloc((size_t)A065927_L * sizeof(uint32_t));
    A065927_T[0] = 0;
    for (uint64_t lo = 1; lo < A065927_L; lo += CH) {
        size_t len = CH;
        if (lo + len > A065927_L) len = (size_t)(A065927_L - lo);
        A065927_seg(lo, len, s, p);
        for (size_t i = 0; i < len; i++) A065927_T[lo + i] = (uint32_t)s[i];
    }
    free(s);
    free(p);
}

/* smallest k >= k0 with sopf(k) == sopf(k+m), using the segmented sieve */
static uint64_t A065927_segsearch(uint64_t m, uint64_t k0) {
    size_t B = (size_t)1 << 18;
    size_t len = B + (size_t)m;
    uint64_t *s = (uint64_t *)malloc(len * sizeof(uint64_t));
    uint64_t *p = (uint64_t *)malloc(len * sizeof(uint64_t));
    uint64_t res = 0;
    for (uint64_t lo = k0; ; lo += B) {
        A065927_seg(lo, len, s, p);
        for (size_t i = 0; i < B; i++) {
            if (s[i] == s[i + m]) { res = lo + i; break; }
        }
        if (res) break;
    }
    free(s);
    free(p);
    return res;
}

/* A065925(m) */
static uint64_t A065927_A065925(uint64_t m) {
    const uint32_t *T = A065927_T;
    uint64_t k = 1;
    while (k + m < A065927_L) {
        if (T[k] == T[k + m]) return k;
        k++;
    }
    return A065927_segsearch(m, k);
}

/* ---------- records ---------- */
static uint64_t *A065927_recs = NULL;
static size_t A065927_cnt = 0, A065927_cap = 0;
static uint64_t A065927_m = 1;
static uint64_t A065927_best = 0;

seq_t a(idx_t n) {
    A065927_table();
    while (A065927_cnt < (size_t)n) {
        uint64_t v = A065927_A065925(A065927_m);
        if (v > A065927_best) {
            A065927_best = v;
            if (A065927_cnt == A065927_cap) {
                A065927_cap = A065927_cap ? A065927_cap * 2 : 64;
                A065927_recs = (uint64_t *)realloc(A065927_recs, A065927_cap * sizeof(uint64_t));
            }
            A065927_recs[A065927_cnt++] = v;
        }
        A065927_m++;
    }
    return (seq_t)A065927_recs[(size_t)n - 1];
}
