/*
 * A000418  Number of n-node rooted trees of height 7.
 * https://oeis.org/A000418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A034824(n) - A034823(n) = H_7(n) - H_6(n), where H_h(n)
 *            is the number of n-node rooted trees of height at most h: H_0(n)
 *            = [n=1], H_h(n) = E_h(n-1), and E_h is the Euler transform of
 *            H_{h-1}: E_h(0) = 1, m*E_h(m) = Sum_{k=1..m} c(k)*E_h(m-k), with
 *            c(k) = Sum_{d|k} d*H_{h-1}(d)
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 27, u64 n <= 51, u128 n <= 99,
 *            big any n
 * Helpers:   a418_is_big, alloc_zero, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a418_is_big.h"

typedef unsigned __int128 a418_u128;

/* fixed-width path: exact in 128-bit whenever n*H_7(n) < 2^128 */
static a418_u128 *a418_f6 = NULL, *a418_f7 = NULL;
static idx_t a418_fN = 0;

static void a418_build_fixed(idx_t N) {
    a418_u128 *H = (a418_u128 *)calloc(N + 1, sizeof(a418_u128));
    a418_u128 *c = (a418_u128 *)calloc(N + 1, sizeof(a418_u128));
    a418_u128 *E = (a418_u128 *)calloc(N + 1, sizeof(a418_u128));
    a418_u128 *h6 = (a418_u128 *)calloc(N + 1, sizeof(a418_u128));
    a418_u128 *h7 = (a418_u128 *)calloc(N + 1, sizeof(a418_u128));
    idx_t d, k, m;
    int h;
    H[1] = 1;
    for (h = 1; h <= 7; h++) {
        for (k = 0; k <= N; k++) c[k] = 0;
        for (d = 1; d <= N; d++) {
            if (H[d] == 0) continue;
            a418_u128 v = (a418_u128)d * H[d];
            for (k = d; k <= N; k += d) c[k] += v;
        }
        E[0] = 1;
        for (m = 1; m < N; m++) {
            a418_u128 s = 0;
            for (k = 1; k <= m; k++) s += c[k] * E[m - k];
            E[m] = s / (a418_u128)m;
        }
        H[0] = 0;
        for (m = 1; m <= N; m++) H[m] = E[m - 1];
        if (h == 6) for (m = 0; m <= N; m++) h6[m] = H[m];
        if (h == 7) for (m = 0; m <= N; m++) h7[m] = H[m];
    }
    free(H); free(c); free(E);
    if (a418_f6) free(a418_f6);
    if (a418_f7) free(a418_f7);
    a418_f6 = h6;
    a418_f7 = h7;
    a418_fN = N;
}

/* arbitrary-precision path */
static seq_t *a418_b6 = NULL, *a418_b7 = NULL;
static idx_t a418_bN = 0;

static void a418_build_big(idx_t N) {
    seq_t *H = alloc_zero(N + 1);
    seq_t *c = alloc_zero(N + 1);
    seq_t *E = alloc_zero(N + 1);
    seq_t *h6 = alloc_zero(N + 1);
    seq_t *h7 = alloc_zero(N + 1);
    idx_t d, k, m;
    int h;
    H[1] = 1;
    for (h = 1; h <= 7; h++) {
        for (k = 0; k <= N; k++) c[k] = 0;
        for (d = 1; d <= N; d++) {
            if (H[d] == 0) continue;
            seq_t v = (seq_t)d * H[d];
            for (k = d; k <= N; k += d) c[k] += v;
        }
        E[0] = 1;
        for (m = 1; m < N; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += c[k] * E[m - k];
            E[m] = s / (seq_t)m;
        }
        H[0] = 0;
        for (m = 1; m <= N; m++) H[m] = E[m - 1];
        if (h == 6) for (m = 0; m <= N; m++) h6[m] = H[m];
        if (h == 7) for (m = 0; m <= N; m++) h7[m] = H[m];
    }
    a418_b6 = h6;
    a418_b7 = h7;
    a418_bN = N;
}

seq_t a(idx_t n) {
    static int mode = -1;
    if (n < 1) return 0;
    if (mode < 0) mode = a418_is_big();
    if (mode) {
        if (n > a418_bN) {
            idx_t N = n;
            if (N < 2 * a418_bN) N = 2 * a418_bN;
            if (N < 32) N = 32;
            a418_build_big(N);
        }
        return a418_b7[n] - a418_b6[n];
    } else {
        if (n > a418_fN) {
            idx_t N = n;
            if (N < 2 * a418_fN) N = 2 * a418_fN;
            if (N < 32) N = 32;
            a418_build_fixed(N);
        }
        return u128_to_seq(a418_f7[n] - a418_f6[n]);
    }
}
