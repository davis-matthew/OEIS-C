/*
 * A002298  Denominator of (2/Pi)*Integral_{0..inf} (sin x / x)^n dx.
 * https://oeis.org/A002298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   (2/Pi)*Integral_{0..inf} (sin x/x)^n dx = S(n)/(2^(n-1)*(n-1)!),
 *            S(n) = Sum_{k=0..floor(n/2)} (-1)^k*binomial(n,k)*(n-2k)^(n-1);
 *            a(n) = 2^(n-1)*(n-1)! / gcd(S(n), 2^(n-1)*(n-1)!). For even n =
 *            2m this equals (n-1)!/gcd(T, (n-1)!) with T = Sum_{k=0..m}
 *            (-1)^k*binomial(n,k)*(m-k)^(n-1). The gcd is...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 30,
 *            big any n
 * Helpers:   q_badd, q_bdiv, q_bmul, q_bsub, q_exact, q_inv, q_mm, q_pw,
 *            q_res
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q_badd.h"
#include "../utils/q_bdiv.h"
#include "../utils/q_bmul.h"
#include "../utils/q_bsub.h"
#include "../utils/q_exact.h"
#include "../utils/q_inv.h"
#include "../utils/q_mm.h"
#include "../utils/q_pw.h"
#include "../utils/q_res.h"

seq_t a(idx_t n) {
    int odd = (int)(n & 1);
    uint32_t *spf, *ex = NULL, *tmp = NULL;
    uint64_t *f, *iu;
    size_t exL = 0;
    idx_t i, j, p;
    seq_t r = 1;

    spf = (uint32_t *)calloc((size_t)n + 2, sizeof(uint32_t));
    f = (uint64_t *)calloc((size_t)n + 2, sizeof(uint64_t));
    iu = (uint64_t *)calloc((size_t)n + 2, sizeof(uint64_t));
    for (i = 2; i <= n; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= n; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    for (p = 2; p + 1 <= n; p++) {
        idx_t e = 0, x = n - 1, vS = 0, t;
        int done = 0, lvl;
        if (spf[p] != (uint32_t)p) continue;
        while (x) { x /= p; e += x; }
        if (p == 2 && odd) e += n - 1;
        if (e == 0) continue;

        for (lvl = 0; lvl < 2 && !done; lvl++) {
            uint64_t lim = lvl == 0 ? 0xFFFFFFFFULL : (1ULL << 62);
            uint64_t M = 1, S;
            int K = 0;
            while ((idx_t)K < e && M <= lim / p) { M *= p; K++; }
            S = q_res(n, p, K, M, spf, f, iu);
            if (S != 0) {
                while (S % p == 0) { S /= p; vS++; }
                done = 1;
            } else if ((idx_t)K == e) {
                vS = e;
                done = 1;
            }
        }
        if (!done) {
            if (!ex) {
                ex = q_exact(n, &exL);
                tmp = (uint32_t *)malloc(exL * sizeof(uint32_t));
            }
            {
                uint32_t *w = (uint32_t *)malloc(exL * sizeof(uint32_t));
                memcpy(w, ex, exL * sizeof(uint32_t));
                while (vS < e) {
                    memcpy(tmp, w, exL * sizeof(uint32_t));
                    if (q_bdiv(tmp, exL, (uint32_t)p) != 0) break;
                    memcpy(w, tmp, exL * sizeof(uint32_t));
                    vS++;
                }
                free(w);
            }
        }
        for (t = vS; t < e; t++) r *= (seq_t)p;
    }

    free(spf); free(f); free(iu);
    if (ex) { free(ex); free(tmp); }
    return r;
}
