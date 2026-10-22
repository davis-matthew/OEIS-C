/*
 * A004870  Numbers that are the sum of at most 8 positive 7th powers.
 * https://oeis.org/A004870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search - enumerate all multisets of at most 8 positive 7th
 *            powers with sum <= L, sort and dedupe; L is doubled until at
 *            least n terms are found
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *s_buf = NULL;
static size_t s_cnt = 0, s_cap = 0;
static uint64_t s_pw[64];
static int s_np = 0;
static uint64_t s_lim = 0;

static void push_val(uint64_t v) {
    if (s_cnt == s_cap) {
        s_cap = s_cap ? s_cap * 2 : 1024;
        s_buf = (uint64_t *)realloc(s_buf, s_cap * sizeof(uint64_t));
    }
    s_buf[s_cnt++] = v;
}

static void rec(int start, int left, uint64_t sum) {
    push_val(sum);
    if (left == 0) return;
    for (int i = start; i < s_np; i++) {
        uint64_t t = sum + s_pw[i];
        if (t > s_lim) break;
        rec(i, left - 1, t);
    }
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t *terms = NULL;
static size_t nterms = 0;
static uint64_t cur_lim = 0;

static void build(uint64_t lim) {
    s_lim = lim;
    s_np = 0;
    for (uint64_t b = 1;; b++) {
        uint64_t p = b * b * b * b * b * b * b;
        if (p > lim) break;
        s_pw[s_np++] = p;
    }
    s_cnt = 0;
    rec(0, 8, 0);
    qsort(s_buf, s_cnt, sizeof(uint64_t), cmp_u64);
    size_t w = 0;
    for (size_t r = 0; r < s_cnt; r++)
        if (w == 0 || s_buf[r] != s_buf[w - 1]) s_buf[w++] = s_buf[r];
    free(terms);
    terms = (uint64_t *)malloc((w ? w : 1) * sizeof(uint64_t));
    for (size_t i = 0; i < w; i++) terms[i] = s_buf[i];
    nterms = w;
    cur_lim = lim;
}

seq_t a(idx_t n) {
    if (cur_lim == 0) build((uint64_t)1 << 20);
    while ((uint64_t)nterms < (uint64_t)n) build(cur_lim * 4);
    return (seq_t)terms[n - 1];
}
