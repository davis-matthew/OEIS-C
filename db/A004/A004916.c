/*
 * A004916  Numbers that are the sum of at most 10 positive 11th powers.
 * https://oeis.org/A004916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: build every sum of at most 10 values k^11 with 1 <= k <=
 *            K, keep the distinct sums below (K+1)^11 (that list is
 *            complete), sort them, and increase K until there are at least n
 *            terms; a(n) is the n-th one
 * Range:     u8 n <= 11, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   a004916_cmp, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a004916_cmp.h"

static uint64_t *g_buf = NULL;
static size_t g_len = 0, g_cap = 0;
static uint64_t g_pw[64];
static uint64_t g_limit;
static int g_K;

static void a004916_push(uint64_t v) {
    if (g_len >= g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_buf = (uint64_t *)realloc(g_buf, g_cap * sizeof(uint64_t));
    }
    g_buf[g_len++] = v;
}

static void a004916_gen(int start, int depth, uint64_t sum) {
    int k;
    a004916_push(sum);
    if (depth == 10) return;
    for (k = start; k <= g_K; k++) {
        uint64_t s = sum + g_pw[k];
        if (s >= g_limit) break;
        a004916_gen(k, depth + 1, s);
    }
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t nterms = 0;
    int K, j;

    if (n >= 1 && n <= nterms) return u128_to_seq((unsigned __int128)terms[n - 1]);

    for (K = 1; K < 40; K++) {
        size_t i, m;
        for (j = 1; j <= K + 1; j++) {
            uint64_t p = 1;
            int e;
            for (e = 0; e < 11; e++) p *= (uint64_t)j;
            g_pw[j] = p;
        }
        g_limit = g_pw[K + 1];
        g_K = K;
        g_len = 0;
        a004916_gen(1, 0, 0);
        qsort(g_buf, g_len, sizeof(uint64_t), a004916_cmp);
        m = 0;
        for (i = 0; i < g_len; i++)
            if (m == 0 || g_buf[i] != g_buf[m - 1]) g_buf[m++] = g_buf[i];
        if (m >= n) {
            free(terms);
            terms = (uint64_t *)malloc(m * sizeof(uint64_t));
            for (i = 0; i < m; i++) terms[i] = g_buf[i];
            nterms = m;
            free(g_buf);
            g_buf = NULL;
            g_len = g_cap = 0;
            return u128_to_seq((unsigned __int128)terms[n - 1]);
        }
    }
    return 0;
}
