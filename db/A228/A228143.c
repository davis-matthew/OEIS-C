/*
 * A228143  Determinant of the (n+1) X (n+1) Hankel-type matrix with
 *          (i,j)-entry equal to A005259(i+j) for all i,j = 0,...,n.
 * https://oeis.org/A228143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = det[ A005259(i+j) ]_{i,j=0..n}, A005259(m) = Sum_{k=0..m}
 *            (C(m,k)*C(m+k,k))^2. Big builds: Bareiss fraction-free
 *            elimination. Fixed-width builds: division-free Berkowitz
 *            determinant, exact mod 2^w, with entries from Pascal's triangle
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 32 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static bool A228143_fixed(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0) return true;
    }
    return false;
}

/* exact for big builds */
static seq_t A228143_apery(idx_t n) {
    seq_t t = 1;
    seq_t result = 1;
    for (idx_t k = 0; k < n; k++) {
        t = t * (n - k) / (k + 1);
        t = t * (n + k + 1) / (k + 1);
        result += t * t;
    }
    return result;
}

/* exact mod 2^w: Apery numbers h[0..2n] via Pascal triangle additions */
static void A228143_apery_table(idx_t n, seq_t *h) {
    long long R = 4 * (long long)n + 1;
    long long m, k;
    seq_t *B = mem_new(seq_t, R * R);
    for (m = 0; m < R; m++) {
        B[m * R] = 1;
        for (k = 1; k <= m; k++)
            B[m * R + k] = B[(m - 1) * R + k - 1] + (k <= m - 1 ? B[(m - 1) * R + k] : (seq_t)0);
    }
    for (m = 0; m <= 2 * (long long)n; m++) {
        seq_t s = 0;
        for (k = 0; k <= m; k++) {
            seq_t x = B[m * R + k];
            seq_t y = B[(m + k) * R + k];
            s += x * x * y * y;
        }
        h[m] = s;
    }
    mem_free(B);
}

static seq_t A228143_berkowitz(const seq_t *M, long long N) {
    seq_t *p = mem_new(seq_t, N + 2);
    seq_t *q = mem_new(seq_t, N + 2);
    seq_t *np = mem_new(seq_t, N + 2);
    seq_t *w = mem_new(seq_t, N + 1);
    seq_t *w2 = mem_new(seq_t, N + 1);
    long long r, i, j, k;
    seq_t det;
    p[0] = 1;
    for (r = 1; r <= N; r++) {
        k = r - 1;
        q[0] = 1;
        q[1] = (seq_t)0 - M[k * N + k];
        for (i = 0; i < k; i++) w[i] = M[i * N + k];
        for (j = 0; j < k; j++) {
            seq_t s = 0;
            for (i = 0; i < k; i++) s += M[k * N + i] * w[i];
            q[j + 2] = (seq_t)0 - s;
            for (i = 0; i < k; i++) {
                seq_t t = 0;
                for (long long l = 0; l < k; l++) t += M[i * N + l] * w[l];
                w2[i] = t;
            }
            for (i = 0; i < k; i++) w[i] = w2[i];
        }
        for (i = 0; i <= r; i++) {
            seq_t s = 0;
            for (j = 0; j <= i; j++) {
                if (i - j <= k) s += q[j] * p[i - j];
            }
            np[i] = s;
        }
        for (i = 0; i <= r; i++) p[i] = np[i];
    }
    det = p[N];
    if (N & 1) det = (seq_t)0 - det;
    mem_free(p); mem_free(q); mem_free(np); mem_free(w); mem_free(w2);
    return det;
}

seq_t a(idx_t n) {
    long long N = (long long)n + 1;
    long long i, j, k;
    seq_t *M;
    seq_t res;
    if (A228143_fixed()) {
        seq_t *h;
        if (n > 60) return 0;
        h = mem_new(seq_t, 2 * N + 1);
        A228143_apery_table(n, h);
        M = mem_new(seq_t, N * N);
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++) M[i * N + j] = h[i + j];
        res = A228143_berkowitz(M, N);
        mem_free(M);
        mem_free(h);
        return res;
    }
    {
        seq_t *h = mem_new(seq_t, 2 * N + 1);
        seq_t prev = 1;
        for (i = 0; i <= 2 * (long long)n; i++) h[i] = A228143_apery((idx_t)i);
        M = mem_new(seq_t, N * N);
        for (i = 0; i < N; i++)
            for (j = 0; j < N; j++) M[i * N + j] = h[i + j];
        for (k = 0; k + 1 < N; k++) {
            for (i = k + 1; i < N; i++)
                for (j = k + 1; j < N; j++)
                    M[i * N + j] = (M[i * N + j] * M[k * N + k] - M[i * N + k] * M[k * N + j]) / prev;
            prev = M[k * N + k];
        }
        res = M[(N - 1) * N + (N - 1)];
        mem_free(M);
        mem_free(h);
        return res;
    }
}
