/*
 * A001252  Number of permutations of order n with the length of longest run
 *          equal to 4.
 * https://oeis.org/A001252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P_4(n) - P_3(n), where P_k(n) counts permutations of [n]
 *            whose longest monotone run has at most k elements. P_k is
 *            computed by a DP over U[i][j][s]: permutations of length i that
 *            end at relative rank j with a run of s ascents. Descent-ending
 *            states follow by symmetry j -> i+1-j. P_k(i) = 2...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 448 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* calloc gives zeroed storage, which is a valid zero value for every seq_t build */
static seq_t *zeroed(idx_t len) {
    return mem_new(seq_t, (size_t)len);
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

/* advance U from length i to i+1 in place; returns total of up-ending perms of length i */
static seq_t step(seq_t *U, seq_t *T, idx_t K, idx_t i) {
    idx_t s, j, jp;
    seq_t tot;
    for (s = 0; s < K; s++) {
        seq_t run = 0;
        for (j = 1; j <= i; j++) {
            run += U[j * K + s];
            U[j * K + s] = run;
        }
    }
    T[0] = 0;
    for (j = 1; j <= i; j++) {
        seq_t t = 0;
        for (s = 0; s < K; s++) t += U[j * K + s];
        T[j] = t;
    }
    tot = T[i];
    for (jp = i + 1; jp >= 1; jp--) {
        for (s = K - 1; s >= 1; s--)
            U[jp * K + s] = U[(jp - 1) * K + (s - 1)];
        U[jp * K + 0] = tot - T[i + 1 - jp];
    }
    return tot;
}

static seq_t *U3 = NULL, *U2 = NULL, *T3 = NULL, *T2 = NULL, *res = NULL;
static idx_t cur = 0, cap = 0;

static void ensure(idx_t len) {
    idx_t nc;
    if (len <= cap) return;
    nc = cap * 2;
    if (nc < len) nc = len;
    if (nc < 16) nc = 16;
    if (cap == 0) {
        U3 = zeroed((nc + 1) * 3);
        U2 = zeroed((nc + 1) * 2);
        T3 = zeroed(nc + 1);
        T2 = zeroed(nc + 1);
        res = zeroed(nc + 1);
    } else {
        U3 = grow_zero(U3, (cap + 1) * 3, (nc + 1) * 3);
        U2 = grow_zero(U2, (cap + 1) * 2, (nc + 1) * 2);
        T3 = grow_zero(T3, cap + 1, nc + 1);
        T2 = grow_zero(T2, cap + 1, nc + 1);
        res = grow_zero(res, cap + 1, nc + 1);
    }
    cap = nc;
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    if (cur == 0) {
        ensure(3);
        U3[2 * 3 + 0] = 1;
        U2[2 * 2 + 0] = 1;
        cur = 2;
    }
    while (cur <= n) {
        seq_t t3, t2;
        ensure(cur + 1);
        t3 = step(U3, T3, 3, cur);
        t2 = step(U2, T2, 2, cur);
        res[cur] = (t3 - t2) * 2;
        cur++;
    }
    return res[n];
}
