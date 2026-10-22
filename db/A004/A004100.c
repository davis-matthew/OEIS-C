/*
 * A004100  Number of labeled nonseparable bipartite graphs on n nodes.
 * https://oeis.org/A004100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   let c(m) = Sum_{k=0..m} C(m,k)*2^(k*(m-k)) (2-colored graphs)
 *            and b(m) the connected bipartite graphs: c(m)/2 = Sum_{k=1..m}
 *            C(m-1,k-1)*b(k)*c(m-k). With C'(x) = Sum b(k+1) x^k/k!, L(x) =
 *            log C'(x) = Sum l_k x^k/k!, D(x) = x*C'(x) (coefficients d_j =
 *            j*b(j)), the block decomposition gives L(x) =...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, pw2, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/pw2.h"

static seq_t *cache = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (n < cache_len) return cache[n];

    idx_t N = n, M = n - 1, W = N + 1, i, j, k, m, s;
    seq_t *C = alloc_zero(W * W);
    for (i = 0; i <= N; i++) {
        C[i * W] = 1;
        for (j = 1; j <= i; j++) {
            C[i * W + j] = C[(i - 1) * W + j - 1];
            if (j < i) C[i * W + j] += C[(i - 1) * W + j];
        }
    }

    seq_t *c = alloc_zero(N + 1);
    seq_t *h = alloc_zero(N + 1);
    seq_t *b = alloc_zero(N + 2);
    c[0] = 1;
    for (m = 1; m <= N; m++) {
        for (k = 0; k <= m; k++) c[m] += C[m * W + k] * pw2(k * (m - k));
        for (k = 0; 2 * k < m; k++) h[m] += C[m * W + k] * pw2(k * (m - k));
        if (m % 2 == 0) h[m] += C[m * W + m / 2] * pw2(m * m / 4 - 1);
    }
    for (m = 1; m <= N; m++) {
        b[m] = h[m];
        for (k = 1; k < m; k++) b[m] -= C[(m - 1) * W + k - 1] * b[k] * c[m - k];
    }

    /* Fc[k] = b(k+1), k = 0..M */
    seq_t *Fc = alloc_zero(M + 1);
    for (k = 0; k <= M; k++) Fc[k] = b[k + 1];

    seq_t *l = alloc_zero(M + 1);
    for (s = 0; s < M; s++) {
        l[s + 1] = Fc[s + 1];
        for (k = 0; k < s; k++) l[s + 1] -= C[s * W + k] * l[k + 1] * Fc[s - k];
    }

    seq_t *d = alloc_zero(M + 1);
    for (j = 1; j <= M; j++) d[j] = (seq_t)j * b[j];

    idx_t V = M + 1;
    seq_t *Bn = alloc_zero(V * V);
    Bn[0] = 1;
    for (i = 1; i <= M; i++) {
        for (k = 1; k <= i; k++) {
            seq_t acc = 0;
            for (j = 1; j + k <= i + 1; j++)
                acc += C[(i - 1) * W + j - 1] * d[j] * Bn[(i - j) * V + k - 1];
            Bn[i * V + k] = acc;
        }
    }

    seq_t *beta = alloc_zero(M + 1);
    for (i = 1; i <= M; i++) {
        beta[i] = l[i];
        for (k = 1; k < i; k++) beta[i] -= Bn[i * V + k] * beta[k];
    }

    seq_t *nc = alloc_zero(N + 1);
    nc[0] = 0;
    nc[1] = 0;
    for (i = 2; i <= N; i++) nc[i] = beta[i - 1];
    if (cache) release(cache, cache_len);
    cache = nc;
    cache_len = N + 1;

    release(C, W * W);
    release(c, N + 1);
    release(h, N + 1);
    release(b, N + 2);
    release(Fc, M + 1);
    release(l, M + 1);
    release(d, M + 1);
    release(Bn, V * V);
    release(beta, M + 1);

    return cache[n];
}
