/*
 * A003382  Numbers that are the sum of 4 nonzero 8th powers.
 * https://oeis.org/A003382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate all sums a^8+b^8+c^8+d^8 with 1<=a<=b<=c<=d
 *            that are <= a limit, then sort and dedupe; double the limit
 *            until there are at least n distinct sums
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 2880, u64 n <= 9883,
 *            u128 n <= 9883, big any n
 * Helpers:   a3382_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3382_cmp.h"

static uint64_t *a3382_vals = NULL;
static idx_t a3382_cnt = 0;

static void a3382_build(uint64_t L) {
    uint64_t p[256];
    int m = 0;
    for (int i = 1; i < 256; i++) {
        uint64_t v = 1;
        for (int k = 0; k < 8; k++) v *= (uint64_t)i;
        if (v > L) break;
        p[i] = v;
        m = i;
    }
    size_t cap = 1024, len = 0;
    uint64_t *buf = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (int d = 1; d <= m; d++) {
        uint64_t sd = p[d];
        for (int c = 1; c <= d; c++) {
            uint64_t sc = sd + p[c];
            if (sc > L) break;
            for (int b = 1; b <= c; b++) {
                uint64_t sb = sc + p[b];
                if (sb > L) break;
                for (int a0 = 1; a0 <= b; a0++) {
                    uint64_t s = sb + p[a0];
                    if (s > L) break;
                    if (len == cap) {
                        cap *= 2;
                        buf = (uint64_t *)realloc(buf, cap * sizeof(uint64_t));
                    }
                    buf[len++] = s;
                }
            }
        }
    }
    qsort(buf, len, sizeof(uint64_t), a3382_cmp);
    size_t k = 0;
    for (size_t i = 0; i < len; i++)
        if (k == 0 || buf[i] != buf[k - 1]) buf[k++] = buf[i];
    free(a3382_vals);
    a3382_vals = buf;
    a3382_cnt = (idx_t)k;
}

seq_t a(idx_t n) {
    static uint64_t L = 0;
    if (L == 0) {
        L = (uint64_t)1 << 30;
        a3382_build(L);
    }
    while (a3382_cnt < n && L < ((uint64_t)1 << 61)) {
        L *= 2;
        a3382_build(L);
    }
    if (n >= 1 && n <= a3382_cnt) return (seq_t)a3382_vals[n - 1];
    return 0;
}
