/*
 * A003243  Number of partially achiral trees with n nodes.
 * https://oeis.org/A003243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   with r = A000081 (rooted trees), S(x) = Sum_{k>=1} r(k) x^(2k-1)
 *            = R(x^2)/x, h(x) = S/(1-S) (rooted trees with an axis going down
 *            from the root), and N(n) = number of nonempty sequences of
 *            S-elements of total weight n, counted up to reversal = (h(n) +
 *            palindromes(n))/2 (computed by a recurrence,...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 40, u64 n <= 76, u128 n <= 149,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/A_grow.h"

static seq_t *R_ = 0, *C_ = 0, *G = 0, *M = 0, *D = 0;
static idx_t rlen = 0, rcap = 0, len = 0, cap = 0;

static void ensure(idx_t n)
{
    idx_t nc, nr, i, k, m, j;
    if (n < len) return;
    nc = 2 * len;
    if (nc < n + 1) nc = n + 1;
    if (nc < 16) nc = 16;
    nr = nc / 2 + 3;
    R_ = A_grow(R_, rcap, nr);
    C_ = A_grow(C_, rcap, nr);
    rcap = nr;
    G = A_grow(G, cap, nc);
    M = A_grow(M, cap, nc);
    D = A_grow(D, cap, nc);
    cap = nc;

    if (rlen == 0) { R_[1] = 1; C_[1] = 1; rlen = 2; }
    while (rlen < nr) {
        idx_t mm = rlen - 1, t = mm + 1, d;
        seq_t s = 0, c = 0;
        for (k = 1; k <= mm; k++) s += C_[k] * R_[mm + 1 - k];
        R_[t] = s / (seq_t)mm;
        for (d = 1; d * d <= t; d++) {
            if (t % d == 0) {
                c += R_[d] * (seq_t)d;
                if (d * d != t) c += R_[t / d] * (seq_t)(t / d);
            }
        }
        C_[t] = c;
        rlen++;
    }

    if (len == 0) { G[0] = 1; M[0] = 1; D[0] = 0; len = 1; }
    for (i = len; i < nc; i++) {
        seq_t h = 0, d = 0, N;
        for (k = 1; k <= i; k += 2) h += R_[(k + 1) / 2] * G[i - k];
        G[i] = h;
        if (i % 2 == 0) {
            for (j = 1; 2 * j < i; j += 2) d += R_[(j + 1) / 2] * R_[(i - j + 1) / 2];
            if ((i / 2) & 1) {
                seq_t s = R_[(i / 2 + 1) / 2];
                if (s % 2 == 0) d += (s / 2) * (s - 1);
                else d += s * ((s - 1) / 2);
            }
        }
        D[i] = d;
        N = (i & 1) ? R_[(i + 1) / 2] : (seq_t)0;
        for (m = 2; m <= i; m += 2) N += D[m] * G[i - m];
        for (k = 1; 2 * k <= i; k += 2) N += R_[(k + 1) / 2] * M[i - 2 * k];
        M[i] = N;
    }
    len = nc;
}

seq_t a(idx_t n)
{
    idx_t j;
    ensure(n);
    if (n & 1) {
        seq_t T = 0;
        for (j = 1; 2 * j < n; j++) T += R_[(n - 2 * j + 1) / 2] * G[j];
        return M[n] - T;
    }
    return M[n] - G[n / 2] + R_[n / 2];
}
