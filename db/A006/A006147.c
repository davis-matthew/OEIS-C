/*
 * A006147  Reversion of Ramanujan numbers.
 * https://oeis.org/A006147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * [x^(n-1)] Product_{k>=1} (1-x^k)^(-24n) (Lagrange
 *            inversion of REVERT(A000594)); coefficients g_j of P^m, P =
 *            Product 1/(1-x^k), m = 24n: g_0 = 1, g_j = (m/j) * Sum_{k=1..j}
 *            sigma(k) g_{j-k}. Fixed-width builds use the division-free
 *            reversion recurrence g_m = -Sum_{k=2..m} tau(k) [...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/u128_to_seq.h"

#define FW_N 40

static bool fw_done = false;
static unsigned __int128 fw_g[FW_N + 1];

static void fw_compute(void) {
    static unsigned __int128 E[FW_N], T[FW_N], U[FW_N], tau[FW_N + 1];
    static unsigned __int128 pw[FW_N + 1][FW_N + 1];
    int i, j, k, m, r;
    for (i = 0; i < FW_N; i++) E[i] = 0;
    E[0] = 1;
    for (k = 1; k < FW_N; k++)
        for (i = FW_N - 1; i >= k; i--) E[i] -= E[i - k];
    for (i = 0; i < FW_N; i++) T[i] = 0;
    T[0] = 1;
    for (r = 0; r < 24; r++) {
        for (i = 0; i < FW_N; i++) {
            unsigned __int128 s = 0;
            for (j = 0; j <= i; j++) s += T[j] * E[i - j];
            U[i] = s;
        }
        for (i = 0; i < FW_N; i++) T[i] = U[i];
    }
    for (k = 1; k <= FW_N; k++) tau[k] = T[k - 1];
    for (i = 0; i <= FW_N; i++)
        for (j = 0; j <= FW_N; j++) pw[i][j] = 0;
    for (i = 0; i <= FW_N; i++) fw_g[i] = 0;
    fw_g[1] = 1;
    pw[1][1] = 1;
    for (m = 2; m <= FW_N; m++) {
        unsigned __int128 c = 0;
        for (k = 2; k <= m; k++) {
            unsigned __int128 s = 0;
            for (i = k - 1; i <= m - 1; i++) s += pw[k - 1][i] * fw_g[m - i];
            pw[k][m] = s;
            c += tau[k] * s;
        }
        fw_g[m] = (unsigned __int128)0 - c;
        pw[1][m] = fw_g[m];
    }
    fw_done = true;
}

static seq_t *big_cache = NULL;
static bool *big_known = NULL;
static idx_t big_len = 0;

static seq_t big_compute(idx_t n) {
    idx_t j, k, d;
    uint64_t *sig = (uint64_t *)calloc(n + 1, sizeof(uint64_t));
    seq_t *g = alloc_zero(n);
    seq_t m = (seq_t)24 * (seq_t)n;
    seq_t res;
    for (d = 1; d <= n; d++)
        for (k = d; k <= n; k += d) sig[k] += d;
    g[0] = (seq_t)1;
    for (j = 1; j < n; j++) {
        seq_t s = (seq_t)0;
        for (k = 1; k <= j; k++) s += g[j - k] * (seq_t)sig[k];
        g[j] = s * m / (seq_t)j;
    }
    res = g[n - 1] / (seq_t)n;
    release(g, n);
    free(sig);
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (!seq_is_big()) {
        if (n > FW_N) return (seq_t)0; /* a(n) > 2^128 here: cannot fit any fixed-width seq_t */
        if (!fw_done) fw_compute();
        return u128_to_seq(fw_g[n]);
    }
    if (n >= big_len) {
        idx_t nl = n + 1, i;
        seq_t *nc = alloc_zero(nl);
        bool *nk = (bool *)calloc(nl, sizeof(bool));
        for (i = 0; i < big_len; i++) {
            nc[i] = big_cache[i];
            nk[i] = big_known[i];
        }
        if (big_cache != NULL) release(big_cache, big_len);
        free(big_known);
        big_cache = nc;
        big_known = nk;
        big_len = nl;
    }
    if (!big_known[n]) {
        big_cache[n] = big_compute(n);
        big_known[n] = true;
    }
    return big_cache[n];
}
