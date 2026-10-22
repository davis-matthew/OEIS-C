/*
 * A003717  Expansion of e.g.f. sin(tanh(x)) (odd powers only).
 * https://oeis.org/A003717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = d(2*n+1, 0), where d(0, x) = sin(x) and d(k, x) = (1 -
 *            x^2)*d/dx d(k-1, x). Write d(k, x) = P_k(x)*sin(x) +
 *            Q_k(x)*cos(x) with P_0 = 1, Q_0 = 0. Then P_{k+1} =
 *            (1-x^2)*(P_k' - Q_k), Q_{k+1} = (1-x^2)*(Q_k' + P_k), and a(n) =
 *            Q_{2n+1}(0)
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 10, i128 n <= 17,
 *            big any n
 * Helpers:   a_big, from_i128, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/a_big.h"

typedef unsigned __int128 u128;

/* Exact modulo 2^128, which gives every term that fits in a fixed-width seq_t */
static seq_t a_fixed(idx_t n) {
    idx_t m = 2 * n + 1, k, s;
    idx_t len = m + 3;
    u128 *P = (u128 *)calloc(len, sizeof(u128));
    u128 *Q = (u128 *)calloc(len, sizeof(u128));
    u128 *R = (u128 *)calloc(len, sizeof(u128));
    u128 *S = (u128 *)calloc(len, sizeof(u128));
    u128 res;
    P[0] = 1;
    for (s = 1; s <= m; s++) {
        idx_t lim = m - s;
        for (k = 0; k <= lim; k++) {
            R[k] = (u128)(k + 1) * P[k + 1] - Q[k];
            S[k] = (u128)(k + 1) * Q[k + 1] + P[k];
        }
        for (k = 0; k <= lim; k++) {
            P[k] = R[k] - (k >= 2 ? R[k - 2] : (u128)0);
            Q[k] = S[k] - (k >= 2 ? S[k - 2] : (u128)0);
        }
        P[lim + 1] = 0;
        Q[lim + 1] = 0;
    }
    res = Q[0];
    free(P); free(Q); free(R); free(S);
    return from_i128((__int128)res);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
