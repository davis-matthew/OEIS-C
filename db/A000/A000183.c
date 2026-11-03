/*
 * A000183  Number of discordant permutations of length n.
 * https://oeis.org/A000183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (-1)^n*(4*n+f(n)) + (n/(n-1))*((n+1)*a(n-1) +
 *            2*(-1)^n*f(n-1)) - ((2*n)/(n-2))*((n-3)*a(n-2) + (-1)^n*f(n-2))
 *            + (n/(n-3))*((n-5)*a(n-3) + 2*(-1)^(n-1)*f(n-3)) +
 *            (n/(n-4))*(a(n-4) + (-1)^(n-1)*f(n-4)), n >= 7, where f(n) =
 *            F(n-1) + F(n+1) + 2 = Lucas(n) + 2, a(1) = a(2) = a(3) = 0, a(4)
 *            = 1...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 176 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int detect_fixed_width(void)
{
    static int cached = -1;
    if (cached < 0) {
        seq_t x = 1;
        int i;
        cached = 0;
        for (i = 0; i < 300; i++) {
            x = x * 2;
            if (x == 0) { cached = 1; break; }
        }
    }
    return cached;
}

static int in_res(int c, int r0, int r1)
{
    return c == r0 || c == r1;
}

/* exact modulo 2^w; used for fixed-width builds with 4 <= n <= 60 */
static seq_t dp_small(idx_t n)
{
    int N = (int)n;
    seq_t r[64], cur[4][64], nxt[4][64], fact[64], res;
    int opts0[4], optsL[4];
    int a0, b0, i, k, st;

    opts0[0] = -1; opts0[1] = N - 1; opts0[2] = 0; opts0[3] = 1;
    optsL[0] = -1; optsL[1] = N - 2; optsL[2] = N - 1; optsL[3] = 0;
    for (k = 0; k < 64; k++) r[k] = 0;

    for (a0 = 0; a0 < 4; a0++) {
        for (b0 = 0; b0 < 4; b0++) {
            int c0 = opts0[a0], cl = optsL[b0];
            int base;
            if (c0 >= 0 && c0 == cl) continue;
            base = (c0 >= 0) + (cl >= 0);
            for (st = 0; st < 4; st++)
                for (k = 0; k < 64; k++) cur[st][k] = 0;
            cur[0][base] = 1;
            for (i = 1; i <= N - 2; i++) {
                for (st = 0; st < 4; st++)
                    for (k = 0; k < 64; k++) nxt[st][k] = 0;
                for (st = 0; st < 4; st++) {
                    int ua = st >> 1, ub = st & 1;
                    for (k = 0; k + 1 < 64; k++) {
                        seq_t v = cur[st][k];
                        if (v == 0) continue;
                        nxt[ub * 2][k] += v;
                        if (!ua && !in_res(i - 1, c0, cl)) nxt[ub * 2][k + 1] += v;
                        if (!ub && !in_res(i, c0, cl)) nxt[2][k + 1] += v;
                        if (!in_res(i + 1, c0, cl)) nxt[ub * 2 + 1][k + 1] += v;
                    }
                }
                for (st = 0; st < 4; st++)
                    for (k = 0; k < 64; k++) cur[st][k] = nxt[st][k];
            }
            for (st = 0; st < 4; st++)
                for (k = 0; k < 64; k++) r[k] += cur[st][k];
        }
    }

    fact[0] = 1;
    for (k = 1; k <= N; k++) fact[k] = fact[k - 1] * (seq_t)k;
    res = 0;
    for (k = 0; k <= N; k++) {
        seq_t t = fact[N - k] * r[k];
        if (k & 1) res -= t; else res += t;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t A[4], L[4];   /* a(m-3..m), Lucas(m-3..m) */
    static idx_t m = 0;

    if (n <= 3) return 0;
    if (detect_fixed_width() && n <= 60) return dp_small(n);
    if (n == 4) return 1;
    if (n == 5) return 2;
    if (n == 6) return 20;

    if (m == 0 || n < m) {
        m = 6;
        A[0] = 0; A[1] = 1; A[2] = 2; A[3] = 20;
        L[0] = 4; L[1] = 7; L[2] = 11; L[3] = 18;
    }
    while (m < n) {
        idx_t k = m + 1;
        int even = (k % 2 == 0);
        seq_t Ln = L[3] + L[2];
        seq_t f0 = Ln + 2, f1 = L[3] + 2, f2 = L[2] + 2, f3 = L[1] + 2, f4 = L[0] + 2;
        idx_t D  = (k - 1) * (k - 2) * (k - 3) * (k - 4);
        idx_t C1 = k * (k - 2) * (k - 3) * (k - 4);
        idx_t C2 = 2 * k * (k - 1) * (k - 3) * (k - 4);
        idx_t C3 = k * (k - 1) * (k - 2) * (k - 4);
        idx_t C4 = k * (k - 1) * (k - 2) * (k - 3);
        seq_t P = 0, Q = 0, t;

        t = ((seq_t)(4 * k) + f0) * D;
        if (even) P += t; else Q += t;

        P += A[3] * C1 * (idx_t)(k + 1);
        t = f1 * C1 * (idx_t)2;
        if (even) P += t; else Q += t;

        Q += A[2] * C2 * (idx_t)(k - 3);
        t = f2 * C2;
        if (even) Q += t; else P += t;

        P += A[1] * C3 * (idx_t)(k - 5);
        t = f3 * C3 * (idx_t)2;
        if (even) Q += t; else P += t;

        P += A[0] * C4;
        t = f4 * C4;
        if (even) Q += t; else P += t;

        A[0] = A[1]; A[1] = A[2]; A[2] = A[3];
        A[3] = (P - Q) / D;
        L[0] = L[1]; L[1] = L[2]; L[2] = L[3]; L[3] = Ln;
        m = k;
    }
    return A[3];
}
