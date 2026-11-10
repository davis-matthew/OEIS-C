/*
 * A064970  a(1)=1; thereafter, values of n for which r(n)-r(n-1) and
 *          r(n-1)-r(n-2) have the same sign, where r(n) = A005132(n).
 * https://oeis.org/A064970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for k>1, a(k) = k-th n >= 2 with sign(r(n)-r(n-1)) =
 *            sign(r(n-1)-r(n-2)), r = A005132 (Recaman); computed by direct
 *            iteration
 * Range:     u8 n <= 38, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *rs_r = NULL;
static size_t rs_len = 0, rs_cap = 0;
static unsigned char *rs_seen = NULL;
static size_t rs_seen_cap = 0;
static idx_t *rs_res = NULL;
static size_t rs_nres = 0, rs_rescap = 0;

static void rs_mark(uint64_t v) {
    if (v >= rs_seen_cap) {
        size_t nc = rs_seen_cap ? rs_seen_cap : 1024;
        while (nc <= v) nc *= 2;
        unsigned char *p = (unsigned char *)realloc(rs_seen, nc);
        for (size_t i = rs_seen_cap; i < nc; i++) p[i] = 0;
        rs_seen = p;
        rs_seen_cap = nc;
    }
    rs_seen[v] = 1;
}

static bool rs_is_seen(uint64_t v) {
    return v < rs_seen_cap && rs_seen[v];
}

static void rs_push_r(uint64_t v) {
    if (rs_len == rs_cap) {
        rs_cap = rs_cap ? rs_cap * 2 : 1024;
        rs_r = (uint64_t *)realloc(rs_r, rs_cap * sizeof(uint64_t));
    }
    rs_r[rs_len++] = v;
    rs_mark(v);
}

static void rs_push_res(idx_t m) {
    if (rs_nres == rs_rescap) {
        rs_rescap = rs_rescap ? rs_rescap * 2 : 256;
        rs_res = (idx_t *)realloc(rs_res, rs_rescap * sizeof(idx_t));
    }
    rs_res[rs_nres++] = m;
}

seq_t a(idx_t n) {
    if (rs_nres == 0) {
        rs_push_r(0);
        rs_push_res(1);
        rs_push_r(1); /* r(1) = 1 */
    }
    while (rs_nres < (size_t)n) {
        uint64_t m = (uint64_t)rs_len;
        uint64_t prev = rs_r[m - 1];
        uint64_t nv;
        if (prev >= m && !rs_is_seen(prev - m)) nv = prev - m;
        else nv = prev + m;
        rs_push_r(nv);
        /* m >= 2 here */
        bool up1 = rs_r[m] > rs_r[m - 1];
        bool up0 = rs_r[m - 1] > rs_r[m - 2];
        if (up1 == up0) rs_push_res((idx_t)m);
    }
    return (seq_t)rs_res[n - 1];
}
