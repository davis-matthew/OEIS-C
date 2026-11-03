/*
 * A005291  Representation degeneracies for boson strings.
 * https://oeis.org/A005291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Sum_{j>=1} (-1)^(j+1) * x^(j*(j+1)/2) * (1 - x^j)^2 /
 *            Prod_{k>=1} (1 - x^k)^2, i.e. a(n) = Sum_{j>=1} (-1)^(j+1) *
 *            (p2(n-T_j) - 2*p2(n-T_j-j) + p2(n-T_j-2j)), T_j = j*(j+1)/2,
 *            p2(m) = Sum_{i=0..m} A000041(i)*A000041(m-i) (A000712)
 * Range:     u8 n <= 20, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/mem.h"

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

static seq_t *P = NULL;
static idx_t Plen = 0;

static void ensure_p(idx_t N) {
    if (N < Plen) return;
    idx_t newlen = Plen ? Plen : 16;
    while (newlen <= N) newlen *= 2;
    P = grow_zero(P, Plen, newlen);
    idx_t start = Plen;
    Plen = newlen;
    for (idx_t i = start; i < Plen; i++) {
        if (i == 0) { P[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += P[i - g1];
                if (g2 <= i) pos += P[i - g2];
            } else {
                neg += P[i - g1];
                if (g2 <= i) neg += P[i - g2];
            }
        }
        P[i] = pos - neg;
    }
}

static seq_t p2(idx_t m) {
    seq_t s = 0;
    for (idx_t i = 0; i <= m; i++) s += P[i] * P[m - i];
    return s;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_p(n);
    seq_t pos = 0, neg = 0;
    for (idx_t j = 1;; j++) {
        idx_t T = j * (j + 1) / 2;
        if (T > n) break;
        bool odd = (j & 1) != 0;
        /* + q^T */
        {
            seq_t v = p2(n - T);
            if (odd) pos += v; else neg += v;
        }
        /* - 2 q^(T+j) */
        if (T + j <= n) {
            seq_t v = p2(n - T - j);
            v += v;
            if (odd) neg += v; else pos += v;
        }
        /* + q^(T+2j) */
        if (T + 2 * j <= n) {
            seq_t v = p2(n - T - 2 * j);
            if (odd) pos += v; else neg += v;
        }
    }
    return pos - neg;
}
