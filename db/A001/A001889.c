/*
 * A001889  Hit polynomials.
 * https://oeis.org/A001889
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=2..n} (-1)^j * C(j,2) * r_j(n) * (n-j)!, where
 *            r_j(n) is the number of ways to place j non-attacking rooks on
 *            the board {(i,c): c-i in {0,1,2}, 1 <= i,c <= n}. So a(n) counts
 *            permutations p of [n] with exactly 2 indices i where p(i)-i is
 *            0, 1 or 2. The rook numbers come from a row-b...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 23 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* one board row: columns k+1 (al1), k+2 (al2), k+3 (al3); state bits = used cols k+1,k+2 */
static void step_row(seq_t **src, seq_t **dst, idx_t deg, int al1, int al2, int al3) {
    int s;
    idx_t j;
    for (s = 0; s < 4; s++)
        for (j = 0; j <= deg + 1; j++) dst[s][j] = 0;
    for (s = 0; s < 4; s++) {
        int b0 = s & 1, b1 = (s >> 1) & 1;
        for (j = 0; j <= deg; j++) {
            seq_t v = src[s][j];
            if (v == 0) continue;
            dst[b1][j] += v;
            if (al1 && !b0) dst[b1][j + 1] += v;
            if (al2 && !b1) dst[1][j + 1] += v;
            if (al3) dst[b1 | 2][j + 1] += v;
        }
    }
}

static seq_t *zalloc_local(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *memo = NULL;
static idx_t done_n = 1;

static void compute_upto(idx_t target) {
    idx_t L = target + 3;
    seq_t *P[4], *Q[4], *T[4], *U[4];
    seq_t *fact;
    int s;
    idx_t i, m, rows_done = 0;

    for (s = 0; s < 4; s++) {
        P[s] = zalloc_local(L);
        Q[s] = zalloc_local(L);
        T[s] = zalloc_local(L);
        U[s] = zalloc_local(L);
    }
    fact = zalloc_local(L);
    fact[0] = 1;
    for (i = 1; i < L; i++) fact[i] = fact[i - 1] * (seq_t)i;

    if (memo) mem_free(memo);
    memo = zalloc_local(L);

    P[0][0] = 1;
    for (m = 2; m <= target; m++) {
        seq_t pos = 0, neg = 0;
        idx_t j;
        while (rows_done + 2 < m) {
            step_row(P, Q, rows_done, 1, 1, 1);
            for (s = 0; s < 4; s++) {
                seq_t *tmp = P[s];
                P[s] = Q[s];
                Q[s] = tmp;
            }
            rows_done++;
        }
        /* row m-1: columns m-1, m allowed; row m: column m only */
        step_row(P, T, m - 2, 1, 1, 0);
        step_row(T, U, m - 1, 1, 0, 0);
        for (j = 2; j <= m; j++) {
            seq_t R = U[0][j] + U[1][j] + U[2][j] + U[3][j];
            seq_t t;
            if (R == 0) continue;
            t = R * (seq_t)(j * (j - 1) / 2) * fact[m - j];
            if (j & 1) neg += t;
            else pos += t;
        }
        memo[m] = pos - neg;
    }

    for (s = 0; s < 4; s++) {
        mem_free(P[s]);
        mem_free(Q[s]);
        mem_free(T[s]);
        mem_free(U[s]);
    }
    mem_free(fact);
    done_n = target;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    if (memo == NULL || n > done_n) {
        idx_t target = n;
        if (memo != NULL && 2 * done_n > target) target = 2 * done_n;
        compute_upto(target);
    }
    return memo[n];
}
