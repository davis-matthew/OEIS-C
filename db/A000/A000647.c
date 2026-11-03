/*
 * A000647  Alkyl naphthalenes C_{n+10} H_{2n+8} with n+10 carbon atoms.
 * https://oeis.org/A000647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. A(x) = (T(x)^8 + 3*T(x^2)^4)/4 with T = g.f. of A000598,
 *            T(x) = 1 + x*Z(S_3; T). T(x)^2 = T(x^2) + 2*D(x), so A(x) = B^4
 *            + 2*B^3*D + 6*B^2*D^2 + 8*B*D^3 + 4*D^4 with B = T(x^2), where
 *            D(x) = (T(x)^2 - T(x^2))/2 contains no division. A000598 comes
 *            from a multiset DP that also avoids division
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

/* x*(x-1)/2 */
static seq_t a647_ch2(seq_t x) {
    if (x % 2 == 0) return (x / 2) * (x - 1);
    return x * ((x - 1) / 2);
}

/* c*(c+1)*(c+2)/6 */
static seq_t a647_ch3(seq_t c) {
    seq_t f0 = c, f1 = c + 1, f2 = c + 2;
    int r = (int)(long long)(c % 3);
    if (r == 0) f0 /= 3;
    else if (r == 1) f2 /= 3;
    else f1 /= 3;
    if (c % 2 == 0) f0 /= 2;
    else f1 /= 2;
    return f0 * f1 * f2;
}

static seq_t *g_T = 0, *g_S = 0, *g_D = 0, *g_BD = 0, *g_D2 = 0;
static idx_t g_N = 0;
static bool g_have = false;

static void a647_build(idx_t N) {
    idx_t s, m, i, t;
    int j;
    seq_t *M[4];
    seq_t *T = alloc_zero(N + 1);
    for (j = 0; j < 4; j++) {
        M[j] = alloc_zero(N + 1);
        M[j][0] = 1;
    }
    T[0] = 1;
    for (s = 1; s <= N; s++) {
        seq_t c, mc2, mc3;
        T[s] = M[3][s - 1];
        if (s == N) break;
        c = T[s];
        mc2 = a647_ch2(c + 1);
        mc3 = a647_ch3(c);
        for (j = 3; j >= 1; j--) {
            for (m = N; m-- > s;) {
                seq_t add = c * M[j - 1][m - s];
                if (j >= 2 && m >= 2 * s) add += mc2 * M[j - 2][m - 2 * s];
                if (j == 3 && m >= 3 * s) add += mc3 * M[0][m - 3 * s];
                M[j][m] += add;
            }
        }
    }

    {
        idx_t H = N / 2;
        seq_t *S = alloc_zero(H + 1);
        seq_t *D = alloc_zero(N + 1);
        seq_t *BD = alloc_zero(N + 1);
        seq_t *D2 = alloc_zero(N + 1);
        for (m = 0; m <= H; m++) {
            seq_t acc = 0;
            for (i = 0; i <= m; i++) acc += T[i] * T[m - i];
            S[m] = acc;
        }
        for (m = 0; m <= N; m++) {
            seq_t acc = 0;
            for (i = 0; 2 * i < m; i++) acc += T[i] * T[m - i];
            if (m % 2 == 0) acc += a647_ch2(T[m / 2]);
            D[m] = acc;
        }
        for (m = 0; m <= N; m++) {
            seq_t acc = 0;
            for (t = 0; 2 * t <= m; t++) acc += T[t] * D[m - 2 * t];
            BD[m] = acc;
        }
        for (m = 0; m <= N; m++) {
            seq_t acc = 0;
            for (i = 0; 2 * i < m; i++) acc += D[i] * D[m - i];
            acc *= 2;
            if (m % 2 == 0) acc += D[m / 2] * D[m / 2];
            D2[m] = acc;
        }
        g_T = T; g_S = S; g_D = D; g_BD = BD; g_D2 = D2;
    }
    g_N = N;
    g_have = true;
}

seq_t a(idx_t n) {
    idx_t i, t;
    seq_t T1 = 0, T2 = 0, T3 = 0, T4 = 0, T5 = 0;
    if (!g_have || n > g_N) {
        idx_t N = n;
        if (g_have && N < g_N + g_N / 2) N = g_N + g_N / 2;
        if (N < 32) N = 32;
        a647_build(N);
    }
    if (n % 2 == 0) {
        idx_t h = n / 2;
        for (t = 0; t <= h; t++) T1 += g_S[t] * g_S[h - t];
    }
    for (t = 0; 2 * t <= n; t++) {
        T2 += g_S[t] * g_BD[n - 2 * t];
        T3 += g_S[t] * g_D2[n - 2 * t];
    }
    for (i = 0; i <= n; i++) {
        T4 += g_BD[i] * g_D2[n - i];
        T5 += g_D2[i] * g_D2[n - i];
    }
    return T1 + 2 * T2 + 6 * T3 + 8 * T4 + 4 * T5;
}
