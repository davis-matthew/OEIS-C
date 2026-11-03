/*
 * A003369  Numbers that are the sum of 2 positive 7th powers.
 * https://oeis.org/A003369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- collect a^7 + b^7 for 1 <= a <= b <= B, sort, drop
 *            duplicates. The k-th distinct value is correct whenever it is <=
 *            (B+1)^7. B is doubled until a(n) is covered
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 269, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 48 terms through n = 9978 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static void A3369_sift(seq_t *v, idx_t i, idx_t len) {
    for (;;) {
        idx_t l = 2 * i + 1, r = l + 1, m = i;
        if (l < len && v[m] < v[l]) m = l;
        if (r < len && v[m] < v[r]) m = r;
        if (m == i) return;
        seq_t t = v[i]; v[i] = v[m]; v[m] = t;
        i = m;
    }
}

static void A3369_sort(seq_t *v, idx_t len) {
    if (len < 2) return;
    for (idx_t i = len / 2; i-- > 0;) A3369_sift(v, i, len);
    for (idx_t e = len - 1; e > 0; e--) {
        seq_t t = v[0]; v[0] = v[e]; v[e] = t;
        A3369_sift(v, 0, e);
    }
}

static seq_t *A3369_vals = NULL;
static idx_t A3369_nvals = 0;
static idx_t A3369_B = 0;
static bool A3369_inf = false;

static void A3369_build(idx_t B) {
    seq_t *pw = mem_new(seq_t, B + 2);
    bool *ok = (bool *)calloc(B + 2, sizeof(bool));
    for (idx_t x = 1; x <= B + 1; x++) {
        seq_t p = 1;
        bool good = true;
        seq_t sx = (seq_t)x;
        for (int k = 0; k < 7; k++) {
            seq_t q = p * sx;
            if (q / sx != p) good = false;
            p = q;
        }
        pw[x] = p;
        ok[x] = good;
    }
    idx_t cap = B * (B + 1) / 2;
    seq_t *s = mem_new(seq_t, cap ? cap : 1);
    idx_t cnt = 0;
    for (idx_t x = 1; x <= B; x++) {
        if (!ok[x]) break;
        for (idx_t y = x; y <= B; y++) {
            if (!ok[y]) break;
            seq_t t = pw[x] + pw[y];
            if (t < pw[y]) break;
            s[cnt++] = t;
        }
    }
    A3369_sort(s, cnt);
    idx_t u = 0;
    for (idx_t i = 0; i < cnt; i++) {
        if (u == 0 || s[i] != s[u - 1]) {
            s[u] = s[i];
            u++;
        }
    }
    bool inf = !ok[B + 1];
    idx_t valid = u;
    if (!inf) {
        seq_t lim = pw[B + 1];
        valid = 0;
        while (valid < u && s[valid] <= lim) valid++;
    }
    mem_free(A3369_vals);
    A3369_vals = s;
    A3369_nvals = valid;
    A3369_B = B;
    A3369_inf = inf;
    mem_free(pw);
    free(ok);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A3369_B == 0) A3369_build(32);
    for (;;) {
        if (n <= A3369_nvals) return A3369_vals[n - 1];
        if (A3369_inf) return 0;
        A3369_build(A3369_B * 2);
    }
}
