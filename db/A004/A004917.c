/*
 * A004917  Numbers that are the sum of at most 11 positive 11th powers.
 * https://oeis.org/A004917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- enumerate every multiset of at most 11 values b^11
 *            with b <= m, keep the sums below (m+1)^11 (that range is then
 *            complete), sort, remove duplicates, and increase m until there
 *            are at least n terms
 * Range:     u8 n <= 12, u16 n <= 78, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *g_terms = NULL;
static idx_t g_len = 0;
static idx_t g_cap = 0;
static bool g_final = false;

static seq_t *r_buf;
static idx_t r_cnt;

static void rec(const seq_t *pw, int b, int rem, seq_t s, seq_t L, bool inf) {
    if (b == 0) {
        r_buf[r_cnt++] = s;
        return;
    }
    seq_t t = s;
    for (int c = 0; c <= rem; c++) {
        if (c > 0) {
            seq_t nt = t + pw[b];
            if (nt < t) break; /* fixed-width overflow */
            t = nt;
        }
        if (!inf && t >= L) break;
        rec(pw, b - 1, rem - c, t, L, inf);
    }
}

static void sift(seq_t *v, idx_t start, idx_t end) {
    idx_t root = start;
    while (2 * root + 1 < end) {
        idx_t child = 2 * root + 1;
        if (child + 1 < end && v[child] < v[child + 1]) child++;
        if (v[root] < v[child]) {
            seq_t tmp = v[root];
            v[root] = v[child];
            v[child] = tmp;
            root = child;
        } else {
            return;
        }
    }
}

static void heapsort_seq(seq_t *v, idx_t len) {
    if (len < 2) return;
    for (idx_t i = len / 2; i-- > 0;) sift(v, i, len);
    for (idx_t e = len - 1; e > 0; e--) {
        seq_t tmp = v[0];
        v[0] = v[e];
        v[e] = tmp;
        sift(v, 0, e);
    }
}

seq_t a(idx_t n) {
    if (n >= 1 && n <= g_len) return g_terms[n - 1];
    if (g_final) return 0;

    seq_t pw[64];
    for (int m = 1; m < 60; m++) {
        /* powers b^11 for b = 1..m, and limit L = (m+1)^11 */
        int usable = 0;
        bool inf = false;
        seq_t L = 0;
        for (int b = 1; b <= m + 1; b++) {
            seq_t p = 1;
            bool ovf = false;
            for (int i = 0; i < 11; i++) {
                seq_t q = p * (seq_t)b;
                if (q / (seq_t)b != p) { ovf = true; break; }
                p = q;
            }
            if (ovf) { inf = true; break; }
            if (b <= m) { pw[b] = p; usable = b; }
            else L = p;
        }

        uint64_t size = 1;
        for (uint64_t i = 1; i <= 11; i++) size = size * ((uint64_t)usable + i) / i;

        r_buf = alloc_zero((idx_t)size);
        r_cnt = 0;
        seq_t zero = 0;
        rec(pw, usable, 11, zero, L, inf);

        heapsort_seq(r_buf, r_cnt);
        idx_t u = 0;
        for (idx_t i = 0; i < r_cnt; i++) {
            if (u == 0 || r_buf[i] != r_buf[u - 1]) {
                r_buf[u++] = r_buf[i];
            }
        }

        if (g_terms != NULL) release(g_terms, g_cap);
        g_terms = r_buf;
        g_cap = (idx_t)size;
        g_len = u;

        if (inf) g_final = true;
        if (n <= g_len) return g_terms[n - 1];
        if (inf) return 0;
    }
    return 0;
}
