/*
 * A004822  Numbers that are the sum of 11 positive 11th powers.
 * https://oeis.org/A004822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - enumerate nondecreasing 11-tuples of 11th powers with
 *            parts <= m, keep the sums below (m+1)^11 + 11 (all terms in that
 *            range need only parts <= m), then sort and remove duplicates;
 *            increase m until there are at least n terms
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 8010, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/u128_to_seq.h"

static uint64_t *g_buf = NULL;
static size_t g_cnt = 0, g_cap = 0;

static uint64_t p11(uint64_t x) {
    uint64_t r = 1;
    int k;
    for (k = 0; k < 11; k++) r *= x;
    return r;
}

static int cmp_u64(const void *x, const void *y) {
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void push(uint64_t v) {
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_buf = (uint64_t *)realloc(g_buf, g_cap * sizeof(uint64_t));
    }
    g_buf[g_cnt++] = v;
}

static void gen(const uint64_t *pw, uint64_t m, uint64_t lo, uint64_t rem,
                uint64_t sum, uint64_t lim) {
    uint64_t i;
    if (rem == 0) {
        push(sum);
        return;
    }
    for (i = lo; i <= m; i++) {
        if (sum + rem * pw[i] >= lim) break;
        gen(pw, m, i, rem - 1, sum + pw[i], lim);
    }
}

seq_t a(idx_t n) {
    static uint64_t m = 0;
    static size_t uniq = 0;

    while (m == 0 || uniq < n) {
        uint64_t pw[64];
        uint64_t i, lim;
        size_t j, k;
        m++;
        for (i = 0; i <= m + 1; i++) pw[i] = p11(i);
        lim = pw[m + 1] + 11;
        g_cnt = 0;
        gen(pw, m, 1, 11, 0, lim);
        qsort(g_buf, g_cnt, sizeof(uint64_t), cmp_u64);
        k = 0;
        for (j = 0; j < g_cnt; j++) {
            if (k == 0 || g_buf[k - 1] != g_buf[j]) g_buf[k++] = g_buf[j];
        }
        uniq = k;
    }
    return u128_to_seq((unsigned __int128)g_buf[n - 1]);
}
