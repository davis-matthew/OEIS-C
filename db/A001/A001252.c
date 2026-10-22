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
 * Helpers:   grow_zero, step, zeroed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zeroed.h"
#include "../utils/grow_zero.h"
#include "../utils/step.h"

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
