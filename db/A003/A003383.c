/*
 * A003383  Numbers that are the sum of 5 nonzero 8th powers.
 * https://oeis.org/A003383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- for a bound X = B^8 + 4, list every sum b1^8+...+b5^8
 *            <= X with 1 <= b1 <= ... <= b5 <= B, sort and dedupe it, and
 *            grow B until there are at least n terms
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3383_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3383_cmp.h"

static uint64_t a3383_p8[256];
static uint64_t *a3383_vals = NULL;
static size_t a3383_cnt = 0, a3383_cap = 0;
static uint64_t *a3383_done = NULL;
static size_t a3383_done_len = 0;

static void a3383_push(uint64_t v) {
    if (a3383_cnt == a3383_cap) {
        a3383_cap = a3383_cap ? a3383_cap * 2 : 1024;
        a3383_vals = (uint64_t *)realloc(a3383_vals, a3383_cap * sizeof(uint64_t));
    }
    a3383_vals[a3383_cnt++] = v;
}

static void a3383_rec(int k, int minb, uint64_t s, uint64_t X, int B) {
    int b;
    for (b = minb; b <= B; b++) {
        if (a3383_p8[b] > X - s) break;
        if (k == 1) a3383_push(s + a3383_p8[b]);
        else a3383_rec(k - 1, b, s + a3383_p8[b], X, B);
    }
}

seq_t a(idx_t n) {
    int B, i, j;
    if (n < 1) return 0;
    if (a3383_done != NULL && n <= a3383_done_len)
        return (seq_t)a3383_done[n - 1];
    for (i = 0; i < 256; i++) {
        uint64_t v = 1;
        for (j = 0; j < 8; j++) v *= (uint64_t)i;
        a3383_p8[i] = v;
    }
    B = 8;
    for (;;) {
        uint64_t X = a3383_p8[B] + 4;
        size_t u, w;
        a3383_cnt = 0;
        a3383_rec(5, 1, 0, X, B);
        qsort(a3383_vals, a3383_cnt, sizeof(uint64_t), a3383_cmp);
        w = 0;
        for (u = 0; u < a3383_cnt; u++)
            if (w == 0 || a3383_vals[u] != a3383_vals[w - 1])
                a3383_vals[w++] = a3383_vals[u];
        if (w >= n || B >= 250) {
            free(a3383_done);
            a3383_done = (uint64_t *)malloc((w ? w : 1) * sizeof(uint64_t));
            for (u = 0; u < w; u++) a3383_done[u] = a3383_vals[u];
            a3383_done_len = w;
            free(a3383_vals);
            a3383_vals = NULL;
            a3383_cnt = a3383_cap = 0;
            break;
        }
        B = B * 3 / 2 + 1;
        if (B > 250) B = 250;
    }
    if (n > a3383_done_len) return 0;
    return (seq_t)a3383_done[n - 1];
}
