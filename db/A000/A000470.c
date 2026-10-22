/*
 * A000470  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-5 places.
 * https://oeis.org/A000470
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [y^5] Sum_{k=0..n} sigma_{n,k}*(n-k)!*(y-1)^k =
 *            Sum_{k=5..n} (-1)^(k-5)*C(k,5)*(n-k)!*sigma_{n,k}, where Sum
 *            sigma_{n,k} t^k u^n = (1 - 2t^2u^2 - 2t^2(1+t)u^3 + 3t^4u^4) /
 *            ((1 - tu)(1 - (1+2t)u - tu^2 + t^3u^3)). With g_m(t) = [u^m]
 *            1/denominator: g_m = (1+3t)g_{m-1} - 2t^2 g_{m-2} - (t^2+...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define OFF 4

seq_t a(idx_t n_idx) {
    long long n = (long long)n_idx;
    long long L = n + 1 + OFF;
    seq_t *g[5];
    long long r, m, k;
    for (r = 0; r < 5; r++) g[r] = (seq_t *)calloc((size_t)L, sizeof(seq_t));

    /* g_0 = 1 stored in ring slot 0 */
    g[0][OFF] = 1;
    for (m = 1; m <= n; m++) {
        seq_t *c  = g[m % 5];
        seq_t *p1 = g[(m + 4) % 5];
        seq_t *p2 = g[(m + 3) % 5];
        seq_t *p3 = g[(m + 2) % 5];
        seq_t *p4 = g[(m + 1) % 5];
        bool h2 = (m >= 2), h3 = (m >= 3), h4 = (m >= 4);
        for (k = 0; k <= m; k++) {
            long long i = k + OFF;
            seq_t v = p1[i] + 3 * p1[i - 1];
            if (h2) v -= 2 * p2[i - 2];
            if (h3) { v -= p3[i - 2]; v -= p3[i - 3]; }
            if (h4) v += p4[i - 4];
            c[i] = v;
        }
    }

    seq_t *gn  = g[n % 5];
    seq_t *gn2 = g[(n + 3) % 5];
    seq_t *gn3 = g[(n + 2) % 5];
    seq_t *gn4 = g[(n + 1) % 5];

    seq_t result = 0;
    seq_t fact = 1; /* (n-k)! */
    for (k = n; k >= 5; k--) {
        long long i = k + OFF;
        seq_t s = gn[i];
        s -= 2 * gn2[i - 2];
        s -= 2 * gn3[i - 2];
        s -= 2 * gn3[i - 3];
        s += 3 * gn4[i - 4];
        uint64_t kk = (uint64_t)k;
        uint64_t b = kk * (kk - 1) / 2;
        b = b * (kk - 2) / 3;
        b = b * (kk - 3) / 4;
        b = b * (kk - 4) / 5;
        seq_t term = s * fact * (seq_t)b;
        if ((k - 5) % 2) result -= term; else result += term;
        fact *= (seq_t)(n - k + 1);
    }

    for (r = 0; r < 5; r++) free(g[r]);
    return result;
}
