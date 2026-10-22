/*
 * A005407  Number of protruded partitions of n with largest part at most 6.
 * https://oeis.org/A005407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: (1-x)^6/Product_{i=1..6} (1-x-x^i+x^(1+2*i)) - 1; a(n) =
 *            [x^n] of it, computed by power-series division: G(k) = N(k) -
 *            Sum_{j=1..48} D(j)*G(k-j), G(0) = 1, with N = (1-x)^6 and D =
 *            Product_{i=1..6} (1-x-x^i+x^(1+2*i))
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define DDEG 48

static long long Dc[DDEG + 1];
static int d_ready = 0;
static seq_t *G = NULL;
static idx_t g_len = 0;

static void build_d(void) {
    long long tmp[DDEG + 1];
    int i, j;
    for (j = 0; j <= DDEG; j++) Dc[j] = 0;
    Dc[0] = 1;
    for (i = 1; i <= 6; i++) {
        for (j = 0; j <= DDEG; j++) tmp[j] = 0;
        for (j = 0; j <= DDEG; j++) {
            if (Dc[j] == 0) continue;
            tmp[j] += Dc[j];
            if (j + 1 <= DDEG) tmp[j + 1] -= Dc[j];
            if (j + i <= DDEG) tmp[j + i] -= Dc[j];
            if (j + 1 + 2 * i <= DDEG) tmp[j + 1 + 2 * i] += Dc[j];
        }
        for (j = 0; j <= DDEG; j++) Dc[j] = tmp[j];
    }
    d_ready = 1;
}

seq_t a(idx_t n) {
    static const long long N[7] = {1, -6, 15, -20, 15, -6, 1};
    idx_t k, j, newlen;
    if (!d_ready) build_d();
    if (n >= g_len) {
        newlen = n + 1;
        seq_t *ng = (seq_t *)malloc(sizeof(seq_t) * newlen);
        for (k = 0; k < g_len; k++) ng[k] = G[k];
        for (k = g_len; k < newlen; k++) {
            seq_t pos = 0, neg = 0;
            long long nk = (k <= 6) ? N[k] : 0;
            if (nk > 0) pos += (seq_t)nk;
            else if (nk < 0) neg += (seq_t)(-nk);
            for (j = 1; j <= k && j <= DDEG; j++) {
                long long d = Dc[j];
                if (d == 0) continue;
                /* G(k) = N(k) - Sum D(j) G(k-j) */
                if (d < 0) pos += (seq_t)(-d) * ng[k - j];
                else neg += (seq_t)d * ng[k - j];
            }
            ng[k] = pos - neg;
        }
        if (G) free(G);
        G = ng;
        g_len = newlen;
    }
    return G[n];
}
