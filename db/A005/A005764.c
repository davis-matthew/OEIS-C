/*
 * A005764  Coefficients of modular function denoted G_6(tau) by Atkin.
 * https://oeis.org/A005764
 *
 * Offset:    -6    (a(-6) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = [q^n] (13*t - (q d/dq t)/f)/5, where t =
 *            eta(tau)^12/eta(11 tau)^12 = q^-5 * Product_{k>=1}
 *            (1-q^k)^12/(1-q^(11k))^12 and f = eta(tau)^2 eta(11 tau)^2 = q *
 *            Product_{k>=1} (1-q^k)^2 (1-q^(11k))^2 (the function on X0(11)
 *            with a pole of order 6 at infinity and a zero of order 4 at the
 *            cusp 0...
 * Range:     i8 n <= -4, i16 n <= 5, i32 n <= 28, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, div_sparse, mul_sparse, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/div_sparse.h"
#include "../utils/mul_sparse.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t K)
{
    idx_t m, cnt, k, i;
    idx_t *jex, *jco, *pex, *pco;
    idx_t jm = 0, pm = 0;
    seq_t *P, *S, *res;

    /* Jacobi: prod (1-q^k)^3 = sum (-1)^m (2m+1) q^(m(m+1)/2) */
    cnt = 0;
    for (m = 0; m * (m + 1) / 2 < K; m++) cnt++;
    jex = (idx_t *)malloc(sizeof(idx_t) * (size_t)(cnt + 1));
    jco = (idx_t *)malloc(sizeof(idx_t) * (size_t)(cnt + 1));
    for (m = 0; m * (m + 1) / 2 < K; m++) {
        jex[jm] = m * (m + 1) / 2;
        jco[jm] = (m % 2 ? -1 : 1) * (2 * m + 1);
        jm++;
    }

    /* Euler: prod (1-q^k) = sum (-1)^m q^(m(3m-1)/2), generalized pentagonals */
    cnt = 2 * (jm + 2) + 2;
    pex = (idx_t *)malloc(sizeof(idx_t) * (size_t)cnt);
    pco = (idx_t *)malloc(sizeof(idx_t) * (size_t)cnt);
    pex[0] = 0; pco[0] = 1; pm = 1;
    for (m = 1;; m++) {
        idx_t g1 = m * (3 * m - 1) / 2, g2 = m * (3 * m + 1) / 2;
        idx_t s = (m % 2) ? -1 : 1;
        if (g1 >= K) break;
        pex[pm] = g1; pco[pm] = s; pm++;
        if (g2 >= K) break;
        pex[pm] = g2; pco[pm] = s; pm++;
    }

    /* P = prod (1-q^k)^12 / (1-q^(11k))^12 */
    P = alloc_zero(K);
    P[0] = 1;
    for (i = 0; i < 4; i++) mul_sparse(P, K, jex, jco, jm, 1);
    for (i = 0; i < 4; i++) div_sparse(P, K, jex, jco, jm, 11);

    /* S = (q d/dq of q^-5 P) * q^5 / (prod (1-q^k)^2 (1-q^(11k))^2) */
    S = alloc_zero(K);
    for (k = 0; k < K; k++) S[k] = P[k] * (k - 5);
    for (i = 0; i < 2; i++) div_sparse(S, K, pex, pco, pm, 1);
    for (i = 0; i < 2; i++) div_sparse(S, K, pex, pco, pm, 11);

    res = alloc_zero(K);
    for (k = 0; k < K; k++) {
        seq_t v = -S[k];
        if (k >= 1) v += P[k - 1] * 13;
        res[k] = v / 5;
    }

    release(P, K);
    release(S, K);
    free(jex); free(jco); free(pex); free(pco);

    if (cache) release(cache, cache_len);
    cache = res;
    cache_len = K;
}

seq_t a(idx_t n)
{
    idx_t k = n + 6;
    if (k < 0) return 0;
    if (k >= cache_len) {
        idx_t K = 2 * cache_len;
        if (K < k + 1) K = k + 1;
        if (K < 64) K = 64;
        build(K);
    }
    return cache[k];
}
