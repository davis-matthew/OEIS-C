/*
 * A002510  Expansion of a modular function for Gamma_0(15).
 * https://oeis.org/A002510
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [q^(n-6)] prod_{k>=1} (1-q^(15k))^13 / ((1-q^k)
 *            (1-q^(3k))^5 (1-q^(5k))^7), i.e. eta(q^15)^13 / (eta(q)
 *            eta(q^3)^5 eta(q^5)^7), computed by repeated division /
 *            multiplication by Euler pentagonal series
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 59, u64 n <= 161,
 *            u128 n <= 508, big any n
 * Validated: standard, pass, 54 terms through n = 989 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* g := g / prod_{m>=1}(1 - q^(k m)), in place, ascending */
static void a002510_div_f(seq_t *g, idx_t L, idx_t k) {
    for (idx_t j = 0; j < L; j++) {
        seq_t pos = g[j];
        seq_t neg = 0;
        for (idx_t i = 1;; i++) {
            idx_t e1 = k * (i * (3 * i - 1) / 2);
            if (e1 > j) break;
            idx_t e2 = k * (i * (3 * i + 1) / 2);
            if (i & 1) {
                pos += g[j - e1];
                if (e2 <= j) pos += g[j - e2];
            } else {
                neg += g[j - e1];
                if (e2 <= j) neg += g[j - e2];
            }
        }
        g[j] = pos - neg;
    }
}

/* g := g * prod_{m>=1}(1 - q^(k m)), in place, descending */
static void a002510_mul_f(seq_t *g, idx_t L, idx_t k) {
    idx_t j = L;
    while (j-- > 0) {
        seq_t pos = g[j];
        seq_t neg = 0;
        for (idx_t i = 1;; i++) {
            idx_t e1 = k * (i * (3 * i - 1) / 2);
            if (e1 > j) break;
            idx_t e2 = k * (i * (3 * i + 1) / 2);
            if (i & 1) {
                neg += g[j - e1];
                if (e2 <= j) neg += g[j - e2];
            } else {
                pos += g[j - e1];
                if (e2 <= j) pos += g[j - e2];
            }
        }
        g[j] = pos - neg;
    }
}

static seq_t *A002510_tab = NULL;
static idx_t A002510_cap = 0;

static void a002510_build(idx_t L) {
    seq_t *g = mem_new(seq_t, (size_t)L);
    for (idx_t j = 0; j < L; j++) g[j] = 0;
    g[0] = 1;
    int t;
    a002510_div_f(g, L, 1);
    for (t = 0; t < 5; t++) a002510_div_f(g, L, 3);
    for (t = 0; t < 7; t++) a002510_div_f(g, L, 5);
    for (t = 0; t < 13; t++) a002510_mul_f(g, L, 15);
    if (A002510_tab) mem_free(A002510_tab);
    A002510_tab = g;
    A002510_cap = L;
}

seq_t a(idx_t n) {
    if (n < 6) return 0;
    idx_t m = n - 6;
    if (m >= A002510_cap) {
        idx_t L = A002510_cap * 2;
        if (L < m + 1) L = m + 1;
        if (L < 64) L = 64;
        a002510_build(L);
    }
    return A002510_tab[m];
}
