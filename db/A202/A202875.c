/*
 * A202875  Array: row n shows the coefficients of the characteristic
 *          polynomial of the n-th principal submatrix of the symmetric matrix
 *          A202874; by antidiagonals.
 * https://oeis.org/A202875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   row k (k >= 1) lists the coefficients of det(M_k - x*I) in
 *            ascending powers of x, where M_k is the k X k leading principal
 *            submatrix of A202874, M = L*L^T, and L is the lower-triangular
 *            Toeplitz matrix of s(i) = F(i+2) = 1,2,3,5,8,... The polynomials
 *            are built one after another with Berkowitz's d...
 * Range:     i8 n <= 16, i16 n <= 48, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t **A202875_rows = NULL;   /* rows[k] = descending coeffs of det(xI - M_k) */
static size_t A202875_R = 0;
static size_t A202875_cap = 0;

/* out = L^T in (length m) */
static void A202875_LT(const seq_t *in, seq_t *out, size_t m)
{
    size_t i;
    for (i = m; i-- > 0;) {
        seq_t z = in[i];
        if (i + 1 < m) z += out[i + 1] + in[i + 1];
        if (i + 2 < m) z += out[i + 2];
        out[i] = z;
    }
}

/* out = L in (length m) */
static void A202875_L(const seq_t *in, seq_t *out, size_t m)
{
    size_t i;
    for (i = 0; i < m; i++) {
        seq_t y = in[i];
        if (i >= 1) y += out[i - 1] + in[i - 1];
        if (i >= 2) y += out[i - 2];
        out[i] = y;
    }
}

static void A202875_ensure(size_t r)
{
    size_t i, j, k, e;
    if (r <= A202875_R) return;
    if (r + 1 > A202875_cap) {
        size_t nc = (r + 1) * 2;
        seq_t **nr = (seq_t **)malloc(nc * sizeof(seq_t *));
        for (i = 0; i < nc; i++) nr[i] = NULL;
        for (i = 0; i < A202875_cap; i++) nr[i] = A202875_rows[i];
        if (A202875_rows) free(A202875_rows);
        A202875_rows = nr;
        A202875_cap = nc;
    }
    if (A202875_R == 0) {
        A202875_rows[0] = mem_new(seq_t, 1);
        A202875_rows[0][0] = 1;
    }

    size_t N = r;
    seq_t *s = mem_new(seq_t, N + 1);
    s[0] = 1;
    if (N >= 1) s[1] = 2;
    for (i = 2; i <= N; i++) s[i] = s[i - 1] + s[i - 2];

    seq_t *M = mem_new(seq_t, N * N);
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++) {
            seq_t v = s[i] * s[j];
            if (i > 0 && j > 0) v += M[(i - 1) * N + (j - 1)];
            M[i * N + j] = v;
        }

    seq_t *t = mem_new(seq_t, N + 2);
    seq_t *c = mem_new(seq_t, N + 1);
    seq_t *v = mem_new(seq_t, N + 1);
    seq_t *z = mem_new(seq_t, N + 1);

    for (k = A202875_R + 1; k <= r; k++) {
        size_t last = k - 1;
        seq_t *p = A202875_rows[k - 1];
        for (i = 0; i <= k; i++) t[i] = 0;
        t[0] = 1;
        t[1] = -M[last * N + last];
        for (i = 0; i < last; i++) { c[i] = M[i * N + last]; v[i] = c[i]; }
        for (e = 0; e + 2 <= k; e++) {
            seq_t d = 0;
            for (i = 0; i < last; i++) d += c[i] * v[i];
            t[e + 2] = -d;
            if (e + 3 <= k) {
                A202875_LT(v, z, last);
                A202875_L(z, v, last);
            }
        }
        seq_t *q = mem_new(seq_t, k + 1);
        for (i = 0; i <= k; i++) {
            seq_t acc = 0;
            for (j = 0; j < k && j <= i; j++) acc += t[i - j] * p[j];
            q[i] = acc;
        }
        A202875_rows[k] = q;
    }
    mem_free(s); mem_free(M); mem_free(t); mem_free(c); mem_free(v); mem_free(z);
    A202875_R = r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t r = 1;
    while (r * (r + 3) / 2 < n) r++;
    idx_t col = n - 1 - (r - 1) * (r + 2) / 2;
    A202875_ensure((size_t)r);
    seq_t val = A202875_rows[r][r - col];
    if (r & 1) val = -val;
    return val;
}
