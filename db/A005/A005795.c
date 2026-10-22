/*
 * A005795  Degrees of fundamental invariants of Weyl group W(E_7).
 * https://oeis.org/A005795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest degree d = m+1, where the exponents m of
 *            E_7 are read off the positive roots: #{exponents = m} = c(m) -
 *            c(m+1), with c(k) the number of positive roots of height k
 *            (roots generated from the E_7 Cartan matrix)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   find_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include "../utils/find_root.h"

static int built = 0;
static int degs[8];
static int ndeg = 0;

static void build(void)
{
    int A[7][7];
    int roots[128][7];
    int cnt = 0, r, i, j, k;
    int c[64];
    int edges[6][2] = { {0, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {1, 3} };
    int maxh = 0;

    memset(A, 0, sizeof A);
    for (i = 0; i < 7; i++) A[i][i] = 2;
    for (k = 0; k < 6; k++) {
        A[edges[k][0]][edges[k][1]] = -1;
        A[edges[k][1]][edges[k][0]] = -1;
    }

    for (i = 0; i < 7; i++) {
        for (j = 0; j < 7; j++) roots[cnt][j] = (i == j);
        cnt++;
    }

    for (r = 0; r < cnt; r++) {
        for (i = 0; i < 7; i++) {
            int q = 0, ip = 0, p;
            int t[7];
            /* q: how many times alpha_i can be subtracted */
            for (;;) {
                int ok = 1;
                for (j = 0; j < 7; j++) t[j] = roots[r][j];
                t[i] -= q + 1;
                if (t[i] < 0) ok = 0;
                if (ok) {
                    int nz = 0;
                    for (j = 0; j < 7; j++) if (t[j]) nz = 1;
                    if (!nz || !find_root(roots, cnt, t)) ok = 0;
                }
                if (!ok) break;
                q++;
            }
            for (j = 0; j < 7; j++) ip += A[i][j] * roots[r][j];
            p = q - ip;
            if (p > 0) {
                for (j = 0; j < 7; j++) t[j] = roots[r][j];
                t[i] += 1;
                if (!find_root(roots, cnt, t) && cnt < 128) {
                    for (j = 0; j < 7; j++) roots[cnt][j] = t[j];
                    cnt++;
                }
            }
        }
    }

    memset(c, 0, sizeof c);
    for (r = 0; r < cnt; r++) {
        int h = 0;
        for (j = 0; j < 7; j++) h += roots[r][j];
        if (h < 63) c[h]++;
        if (h > maxh) maxh = h;
    }

    ndeg = 0;
    for (k = 1; k <= maxh; k++) {
        int mult = c[k] - c[k + 1];
        while (mult-- > 0 && ndeg < 8)
            degs[ndeg++] = k + 1;
    }
    built = 1;
}

seq_t a(idx_t n)
{
    if (!built) build();
    if (n < 1 || n > (idx_t)ndeg) return 0;
    return (seq_t)degs[n - 1];
}
