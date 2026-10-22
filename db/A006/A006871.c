/*
 * A006871  Exponentiation of g.f. for rooted trees.
 * https://oeis.org/A006871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = E(n+1), where E is the EXP transform of A000081 (rooted
 *            trees): E(0) = 1, E(m) = Sum_{k=1..m} binomial(m-1,k-1) * r(k) *
 *            E(m-k), r(1) = 1, r(m+1) = (1/m) * Sum_{k=1..m} (Sum_{d|k}
 *            d*r(d)) * r(m-k+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 37,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"

static seq_t *R = 0;   /* R[m] = A000081(m), m >= 1 */
static seq_t *S = 0;   /* S[m] = Sum_{d|m} d*R[d] */
static seq_t *E = 0;   /* E[m] = EXP transform term */
static seq_t *P = 0;   /* current Pascal row */
static idx_t cap = 0;  /* allocated length of the arrays */
static idx_t have = 0; /* E[0..have-1], R/S[1..have-1] are computed */

static void ensure(idx_t m) {
    idx_t i, k;
    if (m < have) return;
    if (m + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < m + 1) nc *= 2;
        R = grow_zero(R, cap, nc);
        S = grow_zero(S, cap, nc);
        E = grow_zero(E, cap, nc);
        P = grow_zero(P, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        E[0] = 1;
        P[0] = 1; /* row 0 */
        have = 1;
    }
    for (i = have; i <= m; i++) {
        seq_t sum;
        idx_t d;
        /* rooted trees r(i) */
        if (i == 1) {
            R[1] = 1;
        } else {
            sum = 0;
            for (k = 1; k <= i - 1; k++)
                sum += S[k] * R[i - k];
            R[i] = sum / (seq_t)(i - 1);
        }
        /* S[i] = sum_{d|i} d*r(d) */
        sum = 0;
        for (d = 1; d * d <= i; d++) {
            if (i % d == 0) {
                idx_t e = i / d;
                sum += (seq_t)d * R[d];
                if (e != d) sum += (seq_t)e * R[e];
            }
        }
        S[i] = sum;
        /* advance Pascal row to row i-1 */
        if (i >= 2) {
            for (k = i - 1; k >= 1; k--)
                P[k] += P[k - 1];
        }
        /* EXP transform */
        sum = 0;
        for (k = 1; k <= i; k++)
            sum += P[k - 1] * R[k] * E[i - k];
        E[i] = sum;
    }
    have = m + 1;
}

seq_t a(idx_t n) {
    ensure(n + 1);
    return E[n + 1];
}
