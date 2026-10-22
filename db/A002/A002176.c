/*
 * A002176  a(n) = LCM of denominators of Cotesian numbers {C(n,k), 0 <= k <=
 *          n}.
 * https://oeis.org/A002176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   C(n,k) = (1/n) * Integral_{t=0..n} Product_{j<>k} (t-j)/(k-j) dt
 *            (Newton-Cotes weights, A100640/A100641); a(n) = lcm of
 *            denominators of C(n,k), 0<=k<=n. With P(t) = Product_{j=0..n}
 *            (t-j) = Sum p_i t^i and F(x) = Integral_0^n (P(t)-P(x))/(t-x) dt
 *            = Sum_j x^j Sum_{i>j} p_i n^(i-j)/(i-j), we get C(...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   cz_add, cz_addmul, cz_cmp, cz_divs, cz_leg, cz_mul_small, cz_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/cz_add.h"
#include "../utils/cz_addmul.h"
#include "../utils/cz_cmp.h"
#include "../utils/cz_divs.h"
#include "../utils/cz_leg.h"
#include "../utils/cz_mul_small.h"
#include "../utils/cz_sub.h"

seq_t a(idx_t n)
{
    uint64_t N = n, M = N + 1, i, j, k, r, p;
    unsigned lg = 0;
    size_t capP, capW, capF, capN, *pl, *wl, *fl, dl, el, posl, negl, hpl, hnl, nkl, tl;
    uint32_t *P, *W, *F, *Dd, *E, *pos, *neg, *hp, *hn, *nk, *tmp;
    int *fs;
    char *comp;
    uint64_t *best;
    seq_t res = 1;

    while ((M >> lg) != 0) lg++;
    capP = (size_t)((M * lg) / 32 + 3);
    capW = (size_t)((M * lg + 2 * M) / 32 + 4);
    capF = capP + capW + 3;
    capN = capF + (size_t)((N * lg + lg) / 32) + 3;

    /* unsigned Stirling coefficients of Product (t+j), j=0..n */
    P = (uint32_t *)calloc((size_t)(M + 1) * capP, sizeof(uint32_t));
    pl = (size_t *)calloc((size_t)(M + 1), sizeof(size_t));
    P[0] = 1; pl[0] = 1;
    for (j = 0; j <= N; j++) {
        for (i = j + 1; i >= 1; i--) {
            cz_mul_small(P + i * capP, &pl[i], (uint32_t)j);
            cz_add(P + i * capP, &pl[i], P + (i - 1) * capP, pl[i - 1]);
        }
        cz_mul_small(P, &pl[0], (uint32_t)j);
    }

    comp = (char *)calloc((size_t)(M + 1), 1);
    for (i = 2; i * i <= M; i++)
        if (!comp[i])
            for (j = i * i; j <= M; j += i) comp[j] = 1;

    /* D = lcm(1..n+1) */
    Dd = (uint32_t *)calloc(capW, sizeof(uint32_t));
    Dd[0] = 1; dl = 1;
    for (p = 2; p <= M; p++) {
        uint64_t q;
        if (comp[p]) continue;
        q = p;
        while (q * p <= M) q *= p;
        cz_mul_small(Dd, &dl, (uint32_t)q);
    }

    /* W[r-1] = n^r * D / r */
    W = (uint32_t *)calloc((size_t)M * capW, sizeof(uint32_t));
    wl = (size_t *)calloc((size_t)M, sizeof(size_t));
    E = (uint32_t *)calloc(capW, sizeof(uint32_t));
    memcpy(E, Dd, dl * sizeof(uint32_t)); el = dl;
    for (r = 1; r <= M; r++) {
        cz_mul_small(E, &el, (uint32_t)N);
        memcpy(W + (r - 1) * capW, E, el * sizeof(uint32_t));
        wl[r - 1] = el;
        cz_divs(W + (r - 1) * capW, &wl[r - 1], (uint32_t)r);
    }

    /* D*f_j = Sum_r p_{j+r} W[r-1], p_i = (-1)^(M-i) c_i */
    F = (uint32_t *)calloc((size_t)M * capF, sizeof(uint32_t));
    fl = (size_t *)calloc((size_t)M, sizeof(size_t));
    fs = (int *)calloc((size_t)M, sizeof(int));
    pos = (uint32_t *)calloc(capF, sizeof(uint32_t));
    neg = (uint32_t *)calloc(capF, sizeof(uint32_t));
    for (j = 0; j <= N; j++) {
        memset(pos, 0, capF * sizeof(uint32_t));
        memset(neg, 0, capF * sizeof(uint32_t));
        posl = negl = 0;
        for (r = 1; j + r <= M; r++) {
            i = j + r;
            if (pl[i] == 0) continue;
            if ((M - i) & 1)
                cz_addmul(neg, &negl, P + i * capP, pl[i], W + (r - 1) * capW, wl[r - 1]);
            else
                cz_addmul(pos, &posl, P + i * capP, pl[i], W + (r - 1) * capW, wl[r - 1]);
        }
        if (cz_cmp(pos, posl, neg, negl) >= 0) {
            memcpy(F + j * capF, pos, posl * sizeof(uint32_t));
            fl[j] = posl;
            cz_sub(F + j * capF, &fl[j], neg, negl);
            fs[j] = 0;
        } else {
            memcpy(F + j * capF, neg, negl * sizeof(uint32_t));
            fl[j] = negl;
            cz_sub(F + j * capF, &fl[j], pos, posl);
            fs[j] = 1;
        }
    }
    free(P); free(pl); free(W); free(wl); free(E); free(pos); free(neg);

    best = (uint64_t *)calloc((size_t)(M + 1), sizeof(uint64_t));
    hp = (uint32_t *)calloc(capN, sizeof(uint32_t));
    hn = (uint32_t *)calloc(capN, sizeof(uint32_t));
    nk = (uint32_t *)calloc(capN, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(capN, sizeof(uint32_t));

    for (k = 0; k <= N / 2; k++) {
        memset(hp, 0, capN * sizeof(uint32_t));
        memset(hn, 0, capN * sizeof(uint32_t));
        hpl = hnl = 0;
        for (j = N + 1; j-- > 0;) {
            cz_mul_small(hp, &hpl, (uint32_t)k);
            cz_mul_small(hn, &hnl, (uint32_t)k);
            if (fl[j]) {
                if (fs[j]) cz_add(hn, &hnl, F + j * capF, fl[j]);
                else cz_add(hp, &hpl, F + j * capF, fl[j]);
            }
        }
        if (cz_cmp(hp, hpl, hn, hnl) >= 0) {
            memcpy(nk, hp, hpl * sizeof(uint32_t)); nkl = hpl;
            cz_sub(nk, &nkl, hn, hnl);
        } else {
            memcpy(nk, hn, hnl * sizeof(uint32_t)); nkl = hnl;
            cz_sub(nk, &nkl, hp, hpl);
        }
        if (nkl == 0) continue;
        for (p = 2; p <= M; p++) {
            uint64_t vD = 0, q = p, Vden, v = 0, pt = p;
            unsigned t = 1;
            if (comp[p]) continue;
            while (q <= M) { vD++; q *= p; }
            {
                uint64_t x = N, vn = 0;
                while (x % p == 0) { x /= p; vn++; }
                Vden = vD + vn + cz_leg(k, p) + cz_leg(N - k, p);
            }
            while (pt * p < ((uint64_t)1 << 32)) { pt *= p; t++; }
            memcpy(tmp, nk, nkl * sizeof(uint32_t)); tl = nkl;
            while (v < Vden) {
                uint32_t rem = cz_divs(tmp, &tl, (uint32_t)pt);
                if (rem) {
                    while (rem % p == 0) { rem /= (uint32_t)p; v++; }
                    break;
                }
                v += t;
            }
            if (v < Vden && Vden - v > best[p]) best[p] = Vden - v;
        }
    }

    for (p = 2; p <= M; p++) {
        uint64_t e;
        if (comp[p]) continue;
        for (e = 0; e < best[p]; e++) res *= (seq_t)p;
    }

    free(F); free(fl); free(fs); free(Dd); free(comp); free(best);
    free(hp); free(hn); free(nk); free(tmp);
    return res;
}
