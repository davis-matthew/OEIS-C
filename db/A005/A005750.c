/*
 * A005750  Number of planted matched trees with n nodes.
 * https://oeis.org/A005750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = Euler transform of b = A000151, where b(m) =
 *            Sum_{i=1..m} a(i)*a(m+1-i) (B(x) = A(x)^2/x), a(1) = 1. Exact
 *            (arbitrary precision): c(0) = 1, s(m) = Sum_{d|m} d*b(d), c(m) =
 *            (1/m)*Sum_{k=1..m} s(k)*c(m-k), a(m+1) = c(m). Fixed width:
 *            division-free product A(x) = x*Prod_{k>=1} (1-x^k)^(-b(k...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Helpers:   alloc_zero, gcd_v, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"
#include "../utils/gcd_v.h"

/* ---------- arbitrary precision path ---------- */
static seq_t *bA = NULL, *bB = NULL, *bS = NULL, *bC = NULL;
static idx_t bcap = 0;  /* allocated length */
static idx_t btop = 0;  /* largest m with A[m] known */

static void big_extend(idx_t n) {
    if (n + 1 > bcap) {
        idx_t nc = bcap ? bcap : 64;
        while (nc < n + 1) nc *= 2;
        if (bcap == 0) {
            bA = alloc_zero(nc);
            bB = alloc_zero(nc);
            bS = alloc_zero(nc);
            bC = alloc_zero(nc);
            bC[0] = 1;
        } else {
            bA = grow_zero(bA, bcap, nc);
            bB = grow_zero(bB, bcap, nc);
            bS = grow_zero(bS, bcap, nc);
            bC = grow_zero(bC, bcap, nc);
        }
        bcap = nc;
    }
    while (btop < n) {
        idx_t m = btop + 1;
        bA[m] = bC[m - 1];
        seq_t bm = 0;
        for (idx_t i = 1; i <= m; i++) bm += bA[i] * bA[m + 1 - i];
        bB[m] = bm;
        seq_t sm = 0;
        for (idx_t d = 1; d <= m; d++)
            if (m % d == 0) sm += (seq_t)d * bB[d];
        bS[m] = sm;
        seq_t cm = 0;
        for (idx_t k = 1; k <= m; k++) cm += bS[k] * bC[m - k];
        bC[m] = cm / (seq_t)m;
        btop = m;
    }
}

/* ---------- fixed width path ---------- */
#define FIXN 200
static seq_t *fA = NULL;

static void fixed_init(void) {
    idx_t N = FIXN;
    seq_t *P = alloc_zero(N + 1);
    seq_t *B = alloc_zero(N + 1);
    seq_t *bin = alloc_zero(N + 1);
    fA = alloc_zero(N + 1);
    P[0] = 1;
    for (idx_t k = 1; k <= N; k++) {
        fA[k] = P[k - 1];
        seq_t bk = 0;
        for (idx_t i = 1; i <= k; i++) bk += fA[i] * fA[k + 1 - i];
        B[k] = bk;
        idx_t jmax = N / k;
        seq_t mval = 1;
        for (idx_t i = 1; i <= jmax; i++) {
            seq_t g = gcd_v(mval, (seq_t)i);
            mval = (mval / g) * ((bk + (seq_t)(i - 1)) / ((seq_t)i / g));
            bin[i] = mval;
        }
        for (idx_t t = N; t >= k; t--) {
            seq_t add = 0;
            for (idx_t j = 1; k * j <= t; j++) add += bin[j] * P[t - k * j];
            P[t] += add;
        }
    }
    free(P);
    free(B);
    free(bin);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (seq_is_big()) {
        big_extend(n);
        return bA[n];
    }
    if (fA == NULL) fixed_init();
    if (n > FIXN) return 0; /* far beyond any fixed-width range */
    return fA[n];
}
