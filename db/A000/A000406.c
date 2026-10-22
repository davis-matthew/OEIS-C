/*
 * A000406  Coefficients of iterated exponentials.
 * https://oeis.org/A000406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. = L(L(L(L(L(L(x)))))) with L(y) = -log(1-y) (equals the
 *            OEIS e.g.f.). If u = L(f), then (1-f) u' = f', so with e.g.f.
 *            coefficients u(m+1) = f(m+1) + Sum_{k=1..m} binomial(m,k) * f(k)
 *            * u(m+1-k), u(1) = f(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A406_MAXN 10010

static seq_t U[7][A406_MAXN + 2];
static seq_t row[A406_MAXN + 2];
static long long done = 0;

seq_t a(idx_t n) {
    if (n < 1 || n > A406_MAXN) return 0;
    if (done == 0) {
        int lev;
        row[0] = 1;
        for (lev = 1; lev <= 6; lev++) U[lev][1] = 1;
        done = 1;
    }
    while (done < (long long)n) {
        long long m = done, k;
        int lev;
        for (k = m; k >= 1; k--) row[k] += row[k - 1];
        U[1][m + 1] = (seq_t)m * U[1][m];
        for (lev = 2; lev <= 6; lev++) {
            seq_t s = U[lev - 1][m + 1];
            for (k = 1; k <= m; k++)
                s += row[k] * U[lev - 1][k] * U[lev][m + 1 - k];
            U[lev][m + 1] = s;
        }
        done = m + 1;
    }
    return U[6][n];
}
