/*
 * A077343  Final terms of rows in A077341.
 * https://oeis.org/A077343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = A077341(n(n+1)/2), the last of the n terms in row n. Row
 *            n holds the n smallest numbers beginning with n that are not
 *            already used. Only rows whose number is a prefix of n can use
 *            such numbers, so it is a direct search with binary-search
 *            lookups in the cached prefix rows
 * Range:     u8 n <= 11, u16 n <= 110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9082 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t **A077343_rows = NULL;
static idx_t A077343_cap = 0;

static seq_t *A077343_get_row(idx_t r);

static int A077343_in_row(const seq_t *row, idx_t len, seq_t v) {
    idx_t lo = 0, hi = len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (row[mid] < v) lo = mid + 1;
        else hi = mid;
    }
    return lo < len && row[lo] == v;
}

/* generate row r; if out != NULL store all terms; returns last term */
static seq_t A077343_gen(idx_t r, seq_t *out) {
    seq_t *pre[24];
    idx_t plen[24];
    int np = 0;
    idx_t p = r / 10;
    while (p >= 1 && np < 24) {
        pre[np] = A077343_get_row(p);
        plen[np] = p;
        ++np;
        p /= 10;
    }
    idx_t cnt = 0;
    seq_t last = 0;
    seq_t rr = (seq_t)r;
    /* level 0: r itself, then r*10^k + off */
    seq_t pow10 = 1;
    int level = 0;
    while (cnt < r) {
        seq_t base = rr * pow10;
        if (level == 0) {
            int used = 0;
            for (int i = 0; i < np && !used; ++i)
                used = A077343_in_row(pre[i], plen[i], base);
            if (!used) {
                if (out) out[cnt] = base;
                last = base;
                ++cnt;
            }
        } else {
            for (seq_t off = 0; off < pow10 && cnt < r; ++off) {
                seq_t v = base + off;
                int used = 0;
                for (int i = 0; i < np && !used; ++i)
                    used = A077343_in_row(pre[i], plen[i], v);
                if (!used) {
                    if (out) out[cnt] = v;
                    last = v;
                    ++cnt;
                }
            }
        }
        pow10 = (level == 0) ? (seq_t)10 : pow10 * 10;
        ++level;
    }
    return last;
}

static seq_t *A077343_get_row(idx_t r) {
    if (r < A077343_cap && A077343_rows[r]) return A077343_rows[r];
    seq_t *arr = mem_new(seq_t, (size_t)r);
    A077343_gen(r, arr);
    if (r >= A077343_cap) {
        idx_t nc = A077343_cap ? A077343_cap : 16;
        while (nc <= r) nc <<= 1;
        seq_t **t = (seq_t **)malloc((size_t)nc * sizeof(seq_t *));
        if (!t) exit(1);
        for (idx_t i = 0; i < nc; ++i)
            t[i] = (i < A077343_cap) ? A077343_rows[i] : NULL;
        if (A077343_rows) free(A077343_rows);
        A077343_rows = t;
        A077343_cap = nc;
    }
    A077343_rows[r] = arr;
    return arr;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    return A077343_gen(n, NULL);
}
