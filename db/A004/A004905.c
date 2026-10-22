/*
 * A004905  Numbers that are the sum of at most 10 nonzero 10th powers.
 * https://oeis.org/A004905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve/search: enumerate all sums of at most 10 tenth powers b^10
 *            (b >= 1) up to a limit L, sort, dedupe; double L until at least
 *            n distinct terms exist; a(n) = n-th smallest
 * Range:     u8 n <= 11, u16 n <= 115, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/u128_to_seq.h"

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t cur_limit = 0;

static uint64_t *buf = NULL;
static idx_t buf_len = 0, buf_cap = 0;
static uint64_t pw[64];
static int npw = 0;
static uint64_t lim = 0;

static void push(uint64_t v) {
    if (buf_len >= buf_cap) {
        buf_cap = buf_cap ? buf_cap * 2 : 4096;
        buf = (uint64_t *)realloc(buf, (size_t)buf_cap * sizeof(uint64_t));
    }
    buf[buf_len++] = v;
}

static void dfs(int maxb, int remaining, uint64_t sum) {
    int b;
    push(sum);
    if (remaining == 0) return;
    for (b = maxb; b >= 1; b--) {
        if (pw[b] <= lim - sum)
            dfs(b, remaining - 1, sum + pw[b]);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void build(uint64_t L) {
    uint64_t b;
    idx_t i, k;
    lim = L;
    npw = 0;
    for (b = 1; b < 64; b++) {
        uint64_t p = 1;
        bool over = false;
        int e;
        for (e = 0; e < 10; e++) {
            if (p > L / b) { over = true; break; }
            p *= b;
        }
        if (over || p > L) break;
        pw[b] = p;
        npw = (int)b;
    }
    buf_len = 0;
    dfs(npw, 10, 0);
    qsort(buf, (size_t)buf_len, sizeof(uint64_t), cmp_u64);
    k = 0;
    for (i = 0; i < buf_len; i++) {
        if (k == 0 || buf[i] != buf[k - 1]) buf[k++] = buf[i];
    }
    free(terms);
    terms = (uint64_t *)malloc((size_t)(k ? k : 1) * sizeof(uint64_t));
    for (i = 0; i < k; i++) terms[i] = buf[i];
    nterms = k;
    cur_limit = L;
}

seq_t a(idx_t n) {
    if (terms == NULL) build((uint64_t)1 << 20);
    while (n > nterms && cur_limit < ((uint64_t)1 << 62)) {
        build(cur_limit * 2);
    }
    return u128_to_seq((unsigned __int128)terms[n - 1]);
}
