/*
 * A002879  Number of ways of getting a straight flush, 4 of a kind, full
 *          house, flush, straight, 3 of a kind, 2 pair, a pair, nothing in a
 *          7-card poker hand.
 * https://oeis.org/A002879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   exact combinatorial count over the 7-card hands. Category is the
 *            best 5-card hand. Non-flush part: sum over rank-count vectors
 *            (c_0..c_12), sum c_i = 7, c_i <= 4, weighted by prod
 *            binomial(4,c_i), classified by rank pattern. Correction for
 *            flush hands: 4 suits * suited-rank sets F (|F| >= 5) * of...
 * Range:     u8 none, u16 n <= 1, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   a002879_rankcat, a002879_straight
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002879_rankcat.h"
#include "../utils/a002879_straight.h"

static long long a002879_cnt[10];
static int a002879_done = 0;

static void a002879_rec(int r, int left, int *c, long long w) {
    static const long long b4[5] = {1, 4, 6, 4, 1};
    int k;
    if (r == 13) {
        if (left == 0) a002879_cnt[a002879_rankcat(c)] += w;
        return;
    }
    for (k = 0; k <= 4 && k <= left; k++) {
        c[r] = k;
        a002879_rec(r + 1, left - k, c, w * b4[k]);
    }
    c[r] = 0;
}

static void a002879_fix(const int *c, int fcat, long long w) {
    a002879_cnt[a002879_rankcat(c)] -= 4 * w;
    a002879_cnt[fcat] += 4 * w;
}

static void a002879_compute(void) {
    int c[13];
    unsigned F;
    int r, r1, r2, k;
    for (r = 0; r < 13; r++) c[r] = 0;
    a002879_rec(0, 7, c, 1);
    for (F = 0; F < 8192u; F++) {
        int fcat;
        k = 0;
        for (r = 0; r < 13; r++) {
            c[r] = (F >> r) & 1u;
            k += c[r];
        }
        if (k < 5 || k > 7) continue;
        fcat = a002879_straight(F) ? 1 : 4;
        if (k == 7) {
            a002879_fix(c, fcat, 1);
        } else if (k == 6) {
            for (r = 0; r < 13; r++) {
                c[r]++;
                a002879_fix(c, fcat, 3);
                c[r]--;
            }
        } else {
            for (r1 = 0; r1 < 13; r1++) {
                c[r1] += 2;
                a002879_fix(c, fcat, 3);
                c[r1] -= 2;
                for (r2 = r1 + 1; r2 < 13; r2++) {
                    c[r1]++; c[r2]++;
                    a002879_fix(c, fcat, 9);
                    c[r1]--; c[r2]--;
                }
            }
        }
    }
    a002879_done = 1;
}

seq_t a(idx_t n) {
    if (n < 1 || n > 9) return (seq_t)0;
    if (!a002879_done) a002879_compute();
    return (seq_t)(uint64_t)a002879_cnt[(int)n];
}
