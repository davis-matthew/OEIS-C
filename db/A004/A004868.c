/*
 * A004868  Numbers that are the sum of at most 6 positive 7th powers.
 * https://oeis.org/A004868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: enumerate all sums of 6 terms from {0, 1^7, 2^7, ...,
 *            K^7} below (K+1)^7, sort and remove duplicates; enlarge K until
 *            a(n) is reached
 * Range:     u8 n <= 13, u16 n <= 195, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *g_terms = NULL;
static size_t g_nterms = 0;

static uint64_t *g_buf;
static size_t g_cnt;
static uint64_t g_pw[256];
static uint64_t g_limit;
static unsigned g_K;

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void gen(unsigned start, int left, uint64_t sum) {
    unsigned i;
    if (left == 0) {
        g_buf[g_cnt++] = sum;
        return;
    }
    for (i = start; i <= g_K; i++) {
        uint64_t s = sum + g_pw[i];
        if (s >= g_limit) break;
        gen(i, left - 1, s);
    }
}

static void build(unsigned K) {
    unsigned i;
    int j;
    uint64_t cap = 1;
    size_t m, w;
    g_K = K;
    for (i = 0; i <= K; i++) {
        uint64_t p = 1;
        for (j = 0; j < 7; j++) p *= i;
        g_pw[i] = p;
    }
    g_limit = 1;
    for (j = 0; j < 7; j++) g_limit *= (uint64_t)(K + 1);
    /* number of multisets of size 6 from K+1 elements: C(K+6, 6) */
    for (j = 1; j <= 6; j++) cap = cap * (uint64_t)(K + j) / (uint64_t)j;
    g_buf = (uint64_t *)malloc((size_t)cap * sizeof(uint64_t));
    g_cnt = 0;
    gen(0, 6, 0);
    qsort(g_buf, g_cnt, sizeof(uint64_t), cmp_u64);
    w = 0;
    for (m = 0; m < g_cnt; m++) {
        if (w == 0 || g_buf[w - 1] != g_buf[m]) g_buf[w++] = g_buf[m];
    }
    free(g_terms);
    g_terms = g_buf;
    g_nterms = w;
}

seq_t a(idx_t n) {
    unsigned K = (g_terms == NULL) ? 8 : g_K;
    if (g_terms == NULL) build(K);
    while ((uint64_t)g_nterms < (uint64_t)n && K < 250) {
        K += 4;
        if (K > 250) K = 250;
        build(K);
    }
    return (seq_t)g_terms[n - 1];
}
