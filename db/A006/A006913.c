/*
 * A006913  Theta series of laminated lattice LAMBDA_12^{mid}.
 * https://oeis.org/A006913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: A^6 + 38*A^4*C^2 + 52*A^3*C^3 + 33*A^2*C^4 + 4*A*C^5,
 *            where A = theta_3(q^2)^2 and C = theta_2(q^2)^2 (computed as A^6
 *            + B*u^2*(608*B^3 + 3328*B^2*u + 8448*B*u^2 + 4096*u^3), with B =
 *            A and u = C/4 = Sum_{i,j>=0} q^(((2i+1)^2+(2j+1)^2)/2))
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 47, u64 n <= 4001,
 *            u128 n <= 8825, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a006913_cache = NULL;
static idx_t a006913_len = 0;

/* product of two power series truncated to degree N, skipping zero terms */
static seq_t *a006913_mul(const seq_t *x, const seq_t *y, idx_t N)
{
    seq_t *r = alloc_zero(N + 1);
    size_t *nz = (size_t *)malloc((size_t)(N + 1) * sizeof(size_t));
    size_t cnt = 0, t;
    idx_t i, j;
    for (j = 0; j <= N; j++)
        if (y[j] != 0) nz[cnt++] = (size_t)j;
    for (i = 0; i <= N; i++) {
        if (x[i] == 0) continue;
        for (t = 0; t < cnt; t++) {
            j = (idx_t)nz[t];
            if (i + j > N) break;
            r[i + j] += x[i] * y[j];
        }
    }
    free(nz);
    return r;
}

static void a006913_build(idx_t N)
{
    seq_t *B = alloc_zero(N + 1);
    seq_t *u = alloc_zero(N + 1);
    seq_t *B2, *B3, *B6, *H, *T, *res;
    long long x, y, X;
    idx_t k;

    /* B = theta_3(q^2)^2: B[2(x^2+y^2)] counts (x,y) in Z^2 */
    X = 0;
    while ((idx_t)(2 * (X + 1) * (X + 1)) <= N) X++;
    for (x = -X; x <= X; x++)
        for (y = -X; y <= X; y++) {
            idx_t m = (idx_t)(2 * (x * x + y * y));
            if (m <= N) B[m] += 1;
        }
    /* u = theta_2(q^2)^2 / 4 */
    for (x = 1; (idx_t)((x * x + 1) / 2) <= N; x += 2)
        for (y = 1; (idx_t)((x * x + y * y) / 2) <= N; y += 2)
            u[(x * x + y * y) / 2] += 1;

    B2 = a006913_mul(B, B, N);
    B3 = a006913_mul(B2, B, N);
    B6 = a006913_mul(B3, B3, N);

    H = alloc_zero(N + 1);
    for (k = 0; k <= N; k++) H[k] = 4096 * u[k] + 8448 * B[k];
    T = a006913_mul(H, u, N); release(H, N + 1);
    for (k = 0; k <= N; k++) T[k] += 3328 * B2[k];
    H = a006913_mul(T, u, N); release(T, N + 1);
    for (k = 0; k <= N; k++) H[k] += 608 * B3[k];
    T = a006913_mul(H, u, N); release(H, N + 1);
    H = a006913_mul(T, u, N); release(T, N + 1);
    res = a006913_mul(H, B, N); release(H, N + 1);
    for (k = 0; k <= N; k++) res[k] += B6[k];

    release(B, N + 1); release(u, N + 1);
    release(B2, N + 1); release(B3, N + 1); release(B6, N + 1);

    if (a006913_cache) release(a006913_cache, a006913_len);
    a006913_cache = res;
    a006913_len = N + 1;
}

seq_t a(idx_t n)
{
    if (n >= a006913_len) {
        idx_t N = n;
        if (N < 2 * a006913_len) N = 2 * a006913_len;
        if (N < 64) N = 64;
        a006913_build(N);
    }
    return a006913_cache[n];
}
