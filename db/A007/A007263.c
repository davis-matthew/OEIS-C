/*
 * A007263  Coefficients of completely replicable function "6d".
 * https://oeis.org/A007263
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = (eta(q^3)/eta(q^6))^8 + 16*(eta(q^6)/eta(q^3))^8 = q^-1 *
 *            Prod_{k>=1} (1 - q^(3(2k-1)))^8 + 16*q * Prod_{k>=1} (1 +
 *            q^(3k))^8. So a(3m-1) = (-1)^m * [q^m] Prod (1+q^(2k-1))^8,
 *            a(3m+1) = 16 * [q^m] Prod (1+q^k)^8, a(3m) = 0 (m >= 0). The 8th
 *            powers are computed by repeated squaring of serie...
 * Range:     i8 n <= 3, i16 n <= 18, i32 n <= 69, i64 n <= 261, i128 n <= 990,
 *            big any n
 * Helpers:   alloc_zero, release, sq_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/sq_series.h"

static seq_t *Gs = NULL;   /* Prod (1+q^k)^8 */
static seq_t *Rs = NULL;   /* Prod (1+q^(2k-1))^8 */
static idx_t cap = 0;

static void build(idx_t L) {
    idx_t k, i;
    int r;
    seq_t *P = alloc_zero(L);
    seq_t *S = alloc_zero(L);
    seq_t *T = alloc_zero(L);
    P[0] = 1;
    S[0] = 1;
    for (k = 1; k < L; k++)
        for (i = L - 1; i >= k; i--) P[i] += P[i - k];
    for (k = 1; k < L; k += 2)
        for (i = L - 1; i >= k; i--) S[i] += S[i - k];
    for (r = 0; r < 3; r++) {
        sq_series(P, T, L);
        sq_series(S, T, L);
    }
    release(T, L);
    if (Gs) release(Gs, cap);
    if (Rs) release(Rs, cap);
    Gs = P;
    Rs = S;
    cap = L;
}

seq_t a(idx_t n) {
    idx_t m, need;
    if (n < -1) return 0;
    if (n == -1) return 1;
    if (n % 3 == 0) return 0;
    m = (n % 3 == 2) ? (n + 1) / 3 : (n - 1) / 3;
    need = m + 1;
    if (need > cap) {
        idx_t L = need;
        if (L < 2 * cap) L = 2 * cap;
        if (L < 64) L = 64;
        build(L);
    }
    if (n % 3 == 2) {
        if (m % 2 == 1) return -Rs[m];
        return Rs[m];
    }
    return 16 * Gs[m];
}
