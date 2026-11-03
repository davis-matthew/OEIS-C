/*
 * A003391  Numbers that are the sum of 2 positive 9th powers.
 * https://oeis.org/A003391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search - list all sums i^9 + j^9 (1 <= i <= j <= B) below
 *            (B+1)^9, sort and dedupe; enlarge B until n terms are guaranteed
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 65, u64 n <= 9406,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 47 terms through n = 7933 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

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

static bool a3391_pw9(seq_t b, seq_t *out) {
    seq_t p = 1;
    int k;
    for (k = 0; k < 9; k++) {
        seq_t q = p * b;
        if (q / b != p) return false;
        p = q;
    }
    *out = p;
    return true;
}

static seq_t *c_vals = 0;
static idx_t c_cnt = 0;
static idx_t c_B = 0;
static bool c_full = false;

static void a3391_build(idx_t B) {
    seq_t *pw = alloc_zero(B + 2);
    bool *ok = (bool *)malloc((size_t)(B + 2) * sizeof(bool));
    idx_t i, j;
    seq_t bs = 0;
    bool alive = true;
    for (i = 0; i < B + 2; i++) ok[i] = false;
    for (i = 1; i <= B + 1; i++) {
        if (alive) {
            bs++;
            if (bs == 0) alive = false;
            else if (!a3391_pw9(bs, &pw[i])) alive = false;
            else ok[i] = true;
        }
    }
    bool limit_ok = ok[B + 1];
    seq_t *vals = alloc_zero(B * (B + 1) / 2 + 1);
    idx_t cnt = 0;
    for (i = 1; i <= B; i++) {
        if (!ok[i]) break;
        for (j = i; j <= B; j++) {
            if (!ok[j]) break;
            seq_t s = pw[i] + pw[j];
            if (s < pw[j]) break;               /* overflow */
            if (limit_ok && !(s < pw[B + 1])) break;
            vals[cnt++] = s;
        }
    }
    free(ok);
    A3369_sort(vals, cnt);
    idx_t u = 0;
    for (i = 0; i < cnt; i++) {
        if (u == 0 || vals[i] != vals[u - 1]) {
            vals[u] = vals[i];
            u++;
        }
    }
    c_vals = vals;
    c_cnt = u;
    c_B = B;
    c_full = !limit_ok;
}

seq_t a(idx_t n) {
    while (c_vals == 0 || (c_cnt < n && !c_full)) {
        idx_t B = c_B ? c_B * 2 : 64;
        a3391_build(B);
    }
    if (n > c_cnt) return 0;
    return c_vals[n - 1];
}
