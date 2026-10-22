/*
 * A004869  Numbers that are the sum of at most 7 positive 7th powers.
 * https://oeis.org/A004869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search — enumerate all multisets of at most 7 positive 7th
 *            powers with sum <= L, then sort and dedupe the sums; L doubles
 *            until there are at least n terms
 * Range:     u8 n <= 15, u16 n <= 295, u32 n <= 9995, u64 n <= 9995,
 *            u128 n <= 9995, big any n
 * Helpers:   s_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/s_cmp.h"

static uint64_t *s_cache = NULL;
static idx_t s_cache_len = 0;

static uint64_t s_pw[700];
static int s_npw = 0;
static uint64_t s_lim = 0;
static uint64_t *s_buf = NULL;
static size_t s_len = 0, s_cap = 0;

static void s_push(uint64_t v) {
    if (s_len == s_cap) {
        s_cap = s_cap ? s_cap * 2 : 4096;
        s_buf = (uint64_t *)realloc(s_buf, s_cap * sizeof(uint64_t));
    }
    s_buf[s_len++] = v;
}

static void s_rec(int start, int depth, uint64_t sum) {
    s_push(sum);
    if (depth == 7) return;
    for (int i = start; i < s_npw; i++) {
        if (s_pw[i] > s_lim - sum) break;
        s_rec(i, depth + 1, sum + s_pw[i]);
    }
}

seq_t a(idx_t n) {
    if (n >= 1 && n <= s_cache_len) return (seq_t)s_cache[n - 1];

    uint64_t lim = (uint64_t)1 << 20;
    for (;;) {
        s_lim = lim;
        s_npw = 0;
        for (uint64_t k = 1;; k++) {
            uint64_t p = 1;
            int ok = 1;
            for (int i = 0; i < 7; i++) {
                if (p > lim / k) { ok = 0; break; }
                p *= k;
            }
            if (!ok || p > lim) break;
            s_pw[s_npw++] = p;
        }
        s_len = 0;
        s_rec(0, 0, 0);
        qsort(s_buf, s_len, sizeof(uint64_t), s_cmp);
        size_t m = 0;
        for (size_t i = 0; i < s_len; i++) {
            if (m == 0 || s_buf[i] != s_buf[m - 1]) s_buf[m++] = s_buf[i];
        }
        if ((idx_t)m >= n) {
            free(s_cache);
            s_cache = (uint64_t *)malloc(m * sizeof(uint64_t));
            for (size_t i = 0; i < m; i++) s_cache[i] = s_buf[i];
            s_cache_len = (idx_t)m;
            return (seq_t)s_cache[n - 1];
        }
        lim *= 2;
    }
}
