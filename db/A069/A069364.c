/*
 * A069364  Number of 6 X n binary arrays with a path of adjacent 1's from top
 *          row to bottom row.
 * https://oeis.org/A069364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 64^n - (number of 6 X n binary arrays with no 4-connected
 *            path of 1's from top row to bottom row); the latter counted by a
 *            column-by-column transfer-matrix DP over connectivity states
 *            (a(1)=1, a(2)=239)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#define MAXST 20000

static int find_(int *p, int x) {
    while (p[x] != x) { p[x] = p[p[x]]; x = p[x]; }
    return x;
}

static void union_(int *p, int a, int b) {
    a = find_(p, a); b = find_(p, b);
    if (a != b) p[a] = b;
}

static int find_key(const uint64_t *keys, int cnt, uint64_t k) {
    for (int i = 0; i < cnt; i++) if (keys[i] == k) return i;
    return -1;
}

seq_t a(idx_t n) {
    static uint64_t keysA[MAXST], keysB[MAXST];
    uint64_t *cur = keysA, *nxt = keysB;
    seq_t *cc = mem_new(seq_t, MAXST);
    seq_t *nc = mem_new(seq_t, MAXST);
    int ccnt = 1, ncnt;
    cur[0] = 0;
    cc[0] = 1;
    for (idx_t step = 0; step < n; step++) {
        ncnt = 0;
        for (int s = 0; s < ccnt; s++) {
            uint64_t key = cur[s];
            int L[6];
            for (int i = 0; i < 6; i++) L[i] = (int)((key >> (3 * i)) & 7);
            int topf[4], botf[4];
            for (int l = 1; l <= 3; l++) {
                topf[l] = (int)((key >> (18 + l - 1)) & 1);
                botf[l] = (int)((key >> (21 + l - 1)) & 1);
            }
            for (int m = 0; m < 64; m++) {
                int p[9];
                for (int i = 0; i < 9; i++) p[i] = i;
                for (int i = 0; i < 6; i++) {
                    if (!((m >> i) & 1)) continue;
                    if (i > 0 && ((m >> (i - 1)) & 1)) union_(p, 3 + i, 3 + i - 1);
                    if (L[i] > 0) union_(p, 3 + i, L[i] - 1);
                }
                int rt[9], rb[9];
                for (int i = 0; i < 9; i++) rt[i] = rb[i] = 0;
                for (int l = 1; l <= 3; l++) {
                    int r = find_(p, l - 1);
                    if (topf[l]) rt[r] = 1;
                    if (botf[l]) rb[r] = 1;
                }
                if ((m >> 0) & 1) rt[find_(p, 3)] = 1;
                if ((m >> 5) & 1) rb[find_(p, 8)] = 1;
                bool ok = true;
                for (int i = 0; i < 6; i++) {
                    if (!((m >> i) & 1)) continue;
                    int r = find_(p, 3 + i);
                    if (rt[r] && rb[r]) { ok = false; break; }
                }
                if (!ok) continue;
                int map[9];
                for (int i = 0; i < 9; i++) map[i] = 0;
                int nl = 0;
                uint64_t nk = 0;
                for (int i = 0; i < 6; i++) {
                    if (!((m >> i) & 1)) continue;
                    int r = find_(p, 3 + i);
                    if (!map[r]) {
                        map[r] = ++nl;
                        if (rt[r]) nk |= (uint64_t)1 << (18 + nl - 1);
                        if (rb[r]) nk |= (uint64_t)1 << (21 + nl - 1);
                    }
                    nk |= (uint64_t)map[r] << (3 * i);
                }
                int idx = find_key(nxt, ncnt, nk);
                if (idx < 0) {
                    idx = ncnt++;
                    nxt[idx] = nk;
                    nc[idx] = 0;
                }
                nc[idx] += cc[s];
            }
        }
        uint64_t *tk = cur; cur = nxt; nxt = tk;
        seq_t *tc = cc; cc = nc; nc = tc;
        ccnt = ncnt;
    }
    seq_t bad = 0;
    for (int i = 0; i < ccnt; i++) bad += cc[i];
    seq_t total = 1;
    for (idx_t i = 0; i < n; i++) total = total * 64;
    mem_free(cc);
    mem_free(nc);
    return total - bad;
}
