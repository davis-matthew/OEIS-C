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
 * Validated: standard, pass, 33 terms through n = 93 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static seq_t a_big(idx_t n) {
    idx_t m = 2 * n + 1, k, s;
    idx_t len = m + 3;
    seq_t *P = mem_new(seq_t, len);
    seq_t *Q = mem_new(seq_t, len);
    seq_t *R = mem_new(seq_t, len);
    seq_t *S = mem_new(seq_t, len);
    seq_t res;
    for (k = 0; k < len; k++) { P[k] = 0; Q[k] = 0; R[k] = 0; S[k] = 0; }
    P[0] = 1;
    for (s = 1; s <= m; s++) {
        idx_t lim = m - s;
        for (k = 0; k <= lim; k++) {
            R[k] = (seq_t)(k + 1) * P[k + 1] - Q[k];
            S[k] = (seq_t)(k + 1) * Q[k + 1] + P[k];
        }
        for (k = 0; k <= lim; k++) {
            P[k] = R[k];
            Q[k] = S[k];
            if (k >= 2) { P[k] -= R[k - 2]; Q[k] -= S[k - 2]; }
        }
        P[lim + 1] = 0;
        Q[lim + 1] = 0;
    }
    res = Q[0];
    mem_free(P); mem_free(Q); mem_free(R); mem_free(S);
    return res;
}

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
