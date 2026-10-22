/*
 * A006063  A card-arranging problem: values of n such that there exists a
 *          permutation p_1, ..., p_n of 1, ..., n such that i + p_i is a cube
 *          for every i.
 * https://oeis.org/A006063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   grow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: n is a term iff the bipartite graph i ~ j (i + j a cube, 1 <= i,j <= n) has a perfect matching; the maximum matching is kept incrementally, adding left vertex n and then right vertex n, each followed by one augmenting-path search. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/grow_idx.h"

static idx_t M = 0, cap = 0, matched = 0, stamp = 0, lim = 0;
static idx_t *mL = NULL, *mR = NULL, *visL = NULL, *visR = NULL;
static idx_t *good = NULL, ngood = 0, gcap = 0;

static bool aug(idx_t v, idx_t *mateA, idx_t *mateB, idx_t *visB) {
    idx_t c = 1;
    while (c * c * c <= v) c++;
    for (;; c++) {
        idx_t u = c * c * c - v;
        if (u > lim) break;
        if (visB[u] == stamp) continue;
        visB[u] = stamp;
        if (mateB[u] == 0 || aug(mateB[u], mateA, mateB, visB)) {
            mateB[u] = v;
            mateA[v] = u;
            return true;
        }
    }
    return false;
}

static void step(void) {
    if (M + 1 > cap) {
        idx_t nc = cap ? 2 * cap : 1024;
        if (cap == 0) {
            mL = (idx_t *)calloc(nc + 1, sizeof(idx_t));
            mR = (idx_t *)calloc(nc + 1, sizeof(idx_t));
            visL = (idx_t *)calloc(nc + 1, sizeof(idx_t));
            visR = (idx_t *)calloc(nc + 1, sizeof(idx_t));
        } else {
            mL = grow_idx(mL, cap, nc);
            mR = grow_idx(mR, cap, nc);
            visL = grow_idx(visL, cap, nc);
            visR = grow_idx(visR, cap, nc);
        }
        cap = nc;
    }
    M++;
    /* add left vertex M: right side is still 1..M-1 */
    lim = M - 1;
    stamp++;
    if (aug(M, mL, mR, visR)) matched++;
    /* add right vertex M: left side is now 1..M */
    lim = M;
    stamp++;
    if (aug(M, mR, mL, visL)) matched++;
    if (matched == M) {
        if (ngood >= gcap) {
            gcap = gcap ? 2 * gcap : 256;
            good = (idx_t *)realloc(good, gcap * sizeof(idx_t));
        }
        good[ngood++] = M;
    }
}

seq_t a(idx_t n) {
    while (ngood < n) step();
    return (seq_t)good[n - 1];
}
