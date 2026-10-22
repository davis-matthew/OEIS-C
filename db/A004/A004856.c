/*
 * A004856  Numbers that are the sum of at most 5 nonzero 6th powers.
 * https://oeis.org/A004856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- list every sum of at most 5 nonzero 6th powers up to a
 *            limit, then sort and remove duplicates; the limit grows until n
 *            terms are found
 * Range:     u8 n <= 18, u16 n <= 340, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t nterms = 0;
static uint64_t built_limit = 0;

static uint64_t *buf = NULL;
static size_t bufcnt = 0, bufcap = 0;
static uint64_t pw[4096];
static int npw = 0;
static uint64_t lim = 0;

static void push(uint64_t s) {
    if (bufcnt == bufcap) {
        bufcap = bufcap ? bufcap * 2 : 1024;
        buf = (uint64_t *)realloc(buf, bufcap * sizeof(uint64_t));
    }
    buf[bufcnt++] = s;
}

static void rec(int i, int left, uint64_t s) {
    int j;
    push(s);
    if (left == 0) return;
    for (j = i; j < npw && s + pw[j] <= lim; j++)
        rec(j, left - 1, s + pw[j]);
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(uint64_t L) {
    uint64_t b;
    size_t i, k;
    lim = L;
    npw = 0;
    for (b = 1; npw < 4096; b++) {
        uint64_t b3 = b * b * b;
        uint64_t p = b3 * b3;
        if (p > L) break;
        pw[npw++] = p;
    }
    bufcnt = 0;
    rec(0, 5, 0);
    qsort(buf, bufcnt, sizeof(uint64_t), cmp_u64);
    k = 0;
    for (i = 0; i < bufcnt; i++)
        if (k == 0 || buf[i] != buf[k - 1]) buf[k++] = buf[i];
    free(terms);
    terms = (uint64_t *)malloc((k ? k : 1) * sizeof(uint64_t));
    for (i = 0; i < k; i++) terms[i] = buf[i];
    nterms = k;
    built_limit = L;
}

seq_t a(idx_t n) {
    uint64_t L = built_limit ? built_limit : ((uint64_t)1 << 20);
    if (built_limit == 0) build(L);
    while ((uint64_t)nterms < (uint64_t)n) {
        L *= 4;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
