/*
 * A003028  Number of digraphs on n labeled nodes with a source.
 * https://oeis.org/A003028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} C(n,k)*s(k)*g_k(n-k), where s = A003030
 *            (strongly connected digraphs), g_k(m) = 2^(m(m-1)+km) -
 *            Sum_{j=0..m-1} C(m,j)*g_k(j)*2^((m-j)(m-1)), g_k(0) = 1; u(n) =
 *            2^(n(n-1)) - Sum_{t=1..n-1} C(n,t)*u(t)*g_t(n-t); s(n) = u(n) -
 *            Sum_{t=1..n-1} C(n-1,t-1)*s(t)*u(n-t)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   a3028_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a3028_p2.h"

#define A3028_NMAX 256

static seq_t A3028_C[A3028_NMAX + 1][A3028_NMAX + 1];
static seq_t A3028_g[A3028_NMAX + 1][A3028_NMAX + 1];
static seq_t A3028_pw[A3028_NMAX + 1][A3028_NMAX + 1];
static seq_t A3028_u[A3028_NMAX + 1];
static seq_t A3028_s[A3028_NMAX + 1];
static seq_t A3028_a[A3028_NMAX + 1];
static idx_t A3028_done = 0;

static void a3028_step(idx_t n) {
    idx_t t, m, j, k;
    /* Pascal row n */
    A3028_C[n][0] = 1;
    for (k = 1; k <= n; k++) {
        if (n == 0) break;
        A3028_C[n][k] = A3028_C[n - 1][k - 1];
        if (k < n) A3028_C[n][k] += A3028_C[n - 1][k];
    }
    if (n == 0) return;
    /* pw[n][d] = 2^(d(n-1)) */
    {
        seq_t base = a3028_p2(n - 1);
        A3028_pw[n][0] = 1;
        for (j = 1; j <= n; j++) A3028_pw[n][j] = A3028_pw[n][j - 1] * base;
    }
    /* g[t][0] = 1 for t = n; diagonal t + m = n with t, m >= 1 */
    A3028_g[n][0] = 1;
    for (m = 1; m < n; m++) {
        t = n - m;
        seq_t v = A3028_pw[m][m] * a3028_p2(t * m);
        for (j = 0; j < m; j++)
            v -= A3028_C[m][j] * A3028_g[t][j] * A3028_pw[m][m - j];
        A3028_g[t][m] = v;
    }
    /* u(n) */
    {
        seq_t v = A3028_pw[n][n];
        for (t = 1; t < n; t++)
            v -= A3028_C[n][t] * A3028_u[t] * A3028_g[t][n - t];
        A3028_u[n] = v;
    }
    /* s(n) */
    {
        seq_t v = A3028_u[n];
        for (t = 1; t < n; t++)
            v -= A3028_C[n - 1][t - 1] * A3028_s[t] * A3028_u[n - t];
        A3028_s[n] = v;
    }
    /* a(n) */
    {
        seq_t v = 0;
        for (k = 1; k <= n; k++)
            v += A3028_C[n][k] * A3028_s[k] * A3028_g[k][n - k];
        A3028_a[n] = v;
    }
}

seq_t a(idx_t n) {
    if (n < 1 || n > A3028_NMAX) return 0;
    if (A3028_done == 0) {
        a3028_step(0);
        A3028_done = 1;
        a3028_step(1);
    }
    while (A3028_done < n) {
        A3028_done++;
        a3028_step(A3028_done);
    }
    return A3028_a[n];
}
