/*
 * A002434  Theta series of Borcherds' 27-dimensional unimodular lattice T_27.
 * https://oeis.org/A002434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = theta_3^27 - 54*theta_3^19*D + 216*theta_3^11*D^2 -
 *            1024*theta_3^3*D^3, where theta_3 = Sum_{m in Z} q^(m^2) and D =
 *            q*Product_{k>=1} (1-q^(2k-1))^8*(1-q^(4k))^8. Write D =
 *            G*(theta_3^4 - 16*G) with G = theta_2^4/16 = q*psi(q^2)^4 and
 *            psi(x) = Sum_{k>=0} x^(k(k+1)/2). Then a(n) = [q^n] Sum...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 55, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 939 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *a2434_alloc(idx_t N)
{
    seq_t *g = mem_new(seq_t, (size_t)(N + 1));
    idx_t t;
    for (t = 0; t <= N; t++) g[t] = 0;
    return g;
}

static int a2434_fixed(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 300; i++) x = x * 2;
    return x == 0;
}

/* g = f^k, f = 1 + c * Sum_j q^pos[j], via J.C.P. Miller's recurrence (exact division) */
static void a2434_miller(seq_t *g, idx_t N, const idx_t *pos, idx_t cnt, idx_t c, idx_t k)
{
    idx_t n, j;
    g[0] = 1;
    for (n = 1; n <= N; n++) {
        seq_t P = 0, M = 0;
        for (j = 0; j < cnt && pos[j] <= n; j++) {
            idx_t i = pos[j];
            idx_t u = (k + 1) * i;
            if (u >= n) P += g[n - i] * (c * (u - n));
            else M += g[n - i] * (c * (n - u));
        }
        g[n] = (P - M) / n;
    }
}

/* in-place multiply g (length N+1) by 1 + c * Sum_j q^pos[j] */
static void a2434_mul(seq_t *g, idx_t N, const idx_t *pos, idx_t cnt, unsigned c)
{
    idx_t t, j;
    for (t = N + 1; t-- > 1;) {
        seq_t s = g[t];
        for (j = 0; j < cnt && pos[j] <= t; j++) s += g[t - pos[j]] * c;
        g[t] = s;
    }
}

seq_t a(idx_t n)
{
    static seq_t *A[7], *B[7];
    static idx_t cap = 0;
    static int have = 0;
    static const idx_t K[7] = {1, 54, 1080, 7936, 104448, 786432, 4194304};
    seq_t pos = 0, neg = 0;
    int i;

    if (!have || n > cap) {
        idx_t N = n, m, cs = 0, cp = 0, t;
        idx_t *sq, *pr;
        int fixed = a2434_fixed();
        if (have && N < 2 * cap) N = 2 * cap;
        if (N < 64) N = 64;
        if (have) {
            for (i = 0; i < 7; i++) {
                mem_free(A[i]);
                if (i > 0) mem_free(B[i]);
            }
        }
        sq = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
        pr = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
        for (m = 1; m * m <= N; m++) sq[cs++] = m * m;
        for (m = 1; m * (m + 1) <= N; m++) pr[cp++] = m * (m + 1);

        for (i = 0; i < 7; i++) {
            A[i] = a2434_alloc(N);
            if (i > 0) B[i] = a2434_alloc(N);
        }
        if (fixed) {
            int r;
            A[6][0] = 1;
            for (r = 0; r < 3; r++) a2434_mul(A[6], N, sq, cs, 2);
            for (i = 5; i >= 0; i--) {
                for (t = 0; t <= N; t++) A[i][t] = A[i + 1][t];
                for (r = 0; r < 4; r++) a2434_mul(A[i], N, sq, cs, 2);
            }
            B[1][0] = 1;
            for (r = 0; r < 4; r++) a2434_mul(B[1], N, pr, cp, 1);
            for (i = 2; i <= 6; i++) {
                for (t = 0; t <= N; t++) B[i][t] = B[i - 1][t];
                for (r = 0; r < 4; r++) a2434_mul(B[i], N, pr, cp, 1);
            }
        } else {
            for (i = 0; i < 7; i++) {
                a2434_miller(A[i], N, sq, cs, 2, (idx_t)(27 - 4 * i));
                if (i > 0) a2434_miller(B[i], N, pr, cp, 1, (idx_t)(4 * i));
            }
        }
        free(sq);
        free(pr);
        cap = N;
        have = 1;
    }

    for (i = 0; i < 7; i++) {
        seq_t s = 0;
        if (i == 0) {
            s = A[0][n];
        } else {
            idx_t L, m;
            if (n < (idx_t)i) continue;
            L = n - (idx_t)i;
            for (m = 0; m <= L; m++) s += A[i][m] * B[i][L - m];
        }
        if (i % 2 == 0) pos += s * K[i];
        else neg += s * K[i];
    }
    return pos - neg;
}
