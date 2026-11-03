/*
 * A007454  Number of unlabeled disconnected series-parallel posets with n
 *          nodes.
 * https://oeis.org/A007454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003430(n) - A007453(n) for n > 1, a(1) = 1. Let D = x +
 *            P (series-indecomposable SP posets), S = 1/(1-D), Q(n) =
 *            sum_{k=1}^{n-1} D(k)*S(n-k) (connected posets with n >= 2 nodes
 *            are series compositions), C = x + Q (connected), 1 + C + P =
 *            Euler transform (MSET) of C, so a(n) = P(n) = MSET(...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 34, u128 n <= 63,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 961 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

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

static seq_t *Pm = NULL;
static idx_t Pcap = 0;

static void build(idx_t N) {
    seq_t *C = alloc_zero(N + 1);
    seq_t *D = alloc_zero(N + 1);
    seq_t *S = alloc_zero(N + 1);
    seq_t *M = alloc_zero(N + 1);
    seq_t *P = alloc_zero(N + 1);
    seq_t *mc = alloc_zero(N + 1);
    bool knap = seq_is_fixed_width() && N <= 512;
    idx_t n, k, j, m;

    S[0] = 1;
    M[0] = 1;
    for (n = 1; n <= N; n++) {
        seq_t Q = 0;
        for (k = 1; k < n; k++) Q += D[k] * S[n - k];
        C[n] = (n == 1) ? (seq_t)1 : Q;

        if (knap) {
            /* M holds multisets with parts of size <= n-1; add parts of size n */
            mc[0] = 1;
            for (j = 1; j * n <= N; j++) {
                seq_t r = mc[j - 1];
                seq_t t = C[n] + (seq_t)(j - 1);
                seq_t g = gcd(r, (seq_t)j);
                seq_t jj;
                if (g == 0) g = 1;
                r /= g;
                jj = (seq_t)j / g;
                mc[j] = r * (t / jj);
            }
            for (m = N; m >= n; m--) {
                seq_t s = 0;
                for (j = 1; j * n <= m; j++) s += mc[j] * M[m - j * n];
                M[m] += s;
            }
        } else {
            /* Euler transform: mc used as b(k) = sum_{d|k} d*C(d) */
            seq_t s = 0;
            for (m = n; m <= N; m += n) mc[m] += (seq_t)n * C[n];
            for (k = 1; k <= n; k++) s += mc[k] * M[n - k];
            M[n] = s / (seq_t)n;
        }

        P[n] = M[n] - C[n];
        D[n] = P[n] + (seq_t)(n == 1 ? 1 : 0);
        S[n] = Q + D[n];
    }

    if (Pm) release(Pm, Pcap + 1);
    Pm = P;
    Pcap = N;
    release(C, N + 1);
    release(D, N + 1);
    release(S, N + 1);
    release(M, N + 1);
    release(mc, N + 1);
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    if (n > Pcap) {
        idx_t N = n;
        if (!seq_is_fixed_width() && Pcap > 0 && 2 * Pcap > N) N = 2 * Pcap;
        if (N < 32) N = 32;
        build(N);
    }
    return Pm[n];
}
