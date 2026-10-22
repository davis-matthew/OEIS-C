/*
 * A000492  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-6 places.
 * https://oeis.org/A000492
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [y^6] Sum_{k=0..n} sigma_{n,k} (n-k)! (y-1)^k =
 *            Sum_{k=6..n} (-1)^(k-6) C(k,6) (n-k)! sigma_{n,k}, where
 *            sigma_{n,k} is the coefficient of u^n t^k in (1 - 2t^2u^2 -
 *            2t^2(1+t)u^3 + 3t^4u^4) / ((1-tu)(1-(1+2t)u-tu^2+t^3u^3)). With
 *            G = 1/((1-tu)(1-(1+2t)u-tu^2+t^3u^3)) = Sum G_n(t) u^n: G_n =...
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   a492_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a492_add.h"

static seq_t *a492_cache = 0;
static idx_t a492_max = 0;

static void a492_build(idx_t N) {
    seq_t *gp[5], *gn[5];
    int r;
    idx_t i, n, k;
    for (r = 0; r < 5; r++) {
        gp[r] = (seq_t *)calloc(N + 1, sizeof(seq_t));
        gn[r] = (seq_t *)calloc(N + 1, sizeof(seq_t));
        for (i = 0; i <= N; i++) { gp[r][i] = 0; gn[r][i] = 0; }
    }
    seq_t *fact = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *b6 = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *res = (seq_t *)calloc(N + 1, sizeof(seq_t));
    fact[0] = 1;
    for (i = 1; i <= N; i++) fact[i] = fact[i - 1] * (seq_t)i;
    for (i = 0; i <= N; i++) {
        if (i < 6) b6[i] = 0;
        else if (i == 6) b6[i] = 1;
        else b6[i] = b6[i - 1] * (seq_t)i / (seq_t)(i - 6);
        res[i] = 0;
    }

    for (n = 0; n <= N; n++) {
        idx_t cur = n % 5;
        for (i = 0; i <= N; i++) { gp[cur][i] = 0; gn[cur][i] = 0; }
        if (n == 0) {
            gp[cur][0] = 1;
        } else {
            for (k = 0; k <= n; k++) {
                seq_t P = 0, Q = 0;
                a492_add(&P, &Q, gp, gn, n, 1, k, 0, 1);
                a492_add(&P, &Q, gp, gn, n, 1, k, 1, 3);
                a492_add(&P, &Q, gp, gn, n, 2, k, 2, -2);
                a492_add(&P, &Q, gp, gn, n, 3, k, 2, -1);
                a492_add(&P, &Q, gp, gn, n, 3, k, 3, -1);
                a492_add(&P, &Q, gp, gn, n, 4, k, 4, 1);
                if (P >= Q) { gp[cur][k] = P - Q; gn[cur][k] = 0; }
                else { gp[cur][k] = 0; gn[cur][k] = Q - P; }
            }
        }
        if (n >= 6) {
            seq_t apos = 0, aneg = 0;
            for (k = 6; k <= n; k++) {
                seq_t P = 0, Q = 0;
                a492_add(&P, &Q, gp, gn, n, 0, k, 0, 1);
                a492_add(&P, &Q, gp, gn, n, 2, k, 2, -2);
                a492_add(&P, &Q, gp, gn, n, 3, k, 2, -2);
                a492_add(&P, &Q, gp, gn, n, 3, k, 3, -2);
                a492_add(&P, &Q, gp, gn, n, 4, k, 4, 3);
                seq_t s = P - Q;
                seq_t term = s * fact[n - k] * b6[k];
                if (((k - 6) & 1) == 0) apos += term;
                else aneg += term;
            }
            res[n] = apos - aneg;
        }
    }

    for (r = 0; r < 5; r++) { free(gp[r]); free(gn[r]); }
    free(fact);
    free(b6);
    if (a492_cache) free(a492_cache);
    a492_cache = res;
    a492_max = N;
}

seq_t a(idx_t n) {
    if (!a492_cache || n > a492_max) {
        idx_t N = n;
        if (a492_cache && 2 * a492_max > N) N = 2 * a492_max;
        if (N < 32) N = 32;
        a492_build(N);
    }
    return a492_cache[n];
}
