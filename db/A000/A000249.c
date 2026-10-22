/*
 * A000249  Nearest integer to modified Bessel function K_n(5).
 * https://oeis.org/A000249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(K_n(5)), where (DLMF 10.31.1 with z = 5) K_n(z) =
 *            (1/2)(z/2)^(-n) Sum_{k=0..n-1} (n-k-1)!/k! (-z^2/4)^k +
 *            (-1)^(n+1) (log(z/2) + gamma) I_n(z) + (-1)^n (1/2) Sum_{k>=0}
 *            (H(k) + H(n+k)) (z/2)^(n+2k) / (k! (n+k)!), I_n(z) = Sum_{k>=0}
 *            (z/2)^(n+2k) / (k! (n+k)!). It is evaluated in bina...
 * Range:     u8 n <= 12, u16 n <= 15, u32 n <= 21, u64 n <= 30, u128 n <= 46,
 *            big any n
 * Helpers:   fx_add, fx_cmp, fx_div, fx_div2, fx_mul, fx_norm, fx_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/fx_add.h"
#include "../utils/fx_cmp.h"
#include "../utils/fx_div.h"
#include "../utils/fx_div2.h"
#include "../utils/fx_mul.h"
#include "../utils/fx_norm.h"
#include "../utils/fx_sub.h"

#define FB 5   /* fraction limbs (32 bits each) */
#define SC 24  /* limbs for the small correction quantities */

seq_t a(idx_t n)
{
    static const char gam[] = "57721566490153286060651209008240243104215933593992";
    unsigned bl = 0;
    idx_t m = n, k, j;
    while (m) { bl++; m >>= 1; }
    size_t cap = FB + 16 + (size_t)((n * (bl + 2)) / 32);
    uint32_t *t = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(cap, sizeof(uint32_t));
    size_t tl = 0, Pl = 0, Nl = 0, i, u, v;
    seq_t r = 0;

    /* finite alternating sum: t_0 = (n-1)! 2^(n-1) / 5^n,
       t_k = t_{k-1} * 25 / (4 k (n-k)) */
    if (n >= 1) {
        idx_t e = n - 1 + 32 * FB;
        t[e / 32] = (uint32_t)1 << (e % 32);
        tl = (size_t)(e / 32) + 1;
        for (j = 2; j < n; j++) fx_mul(t, &tl, (uint32_t)j);
        k = n;
        while (k >= 13) { fx_div(t, &tl, 1220703125u); k -= 13; }
        {
            uint32_t p = 1;
            while (k > 0) { p *= 5; k--; }
            fx_div(t, &tl, p);
        }
        fx_add(P, &Pl, t, tl);
        for (k = 1; k < n && tl > 0; k++) {
            fx_mul(t, &tl, 25);
            fx_div2(t, &tl, 4 * k, n - k);
            if (k & 1) fx_add(N, &Nl, t, tl);
            else fx_add(P, &Pl, t, tl);
        }
    }

    /* correction terms involving I_n(5), log(5/2), gamma, harmonic numbers */
    {
        uint32_t w[SC], I[SC], H[SC], q[SC], g[SC], c[SC], pr[2 * SC];
        size_t wl, Il = 0, Hl = 0, ql, gl = 0, cl = 0, pl;
        memset(w, 0, sizeof w); memset(I, 0, sizeof I); memset(H, 0, sizeof H);
        memset(q, 0, sizeof q); memset(g, 0, sizeof g); memset(c, 0, sizeof c);
        memset(pr, 0, sizeof pr);

        /* log(5/2) = 2 * Sum (3/7)^(2k+1) / (2k+1) */
        w[FB] = 6; wl = FB + 1;
        fx_div(w, &wl, 7);
        for (k = 0; wl > 0; k++) {
            memcpy(q, w, sizeof q); ql = wl;
            fx_div(q, &ql, (uint32_t)(2 * k + 1));
            fx_add(c, &cl, q, ql);
            fx_mul(w, &wl, 9);
            fx_div(w, &wl, 49);
        }
        /* gamma from its decimal digits */
        for (i = strlen(gam); i > 0; i--) {
            g[FB] = (uint32_t)(gam[i - 1] - '0');
            gl = FB + 1;
            fx_div(g, &gl, 10);
        }
        fx_add(c, &cl, g, gl);

        /* w_k = (5/2)^(n+2k) / (k! (n+k)!) */
        memset(w, 0, sizeof w);
        w[FB] = 1; wl = FB + 1;
        for (j = 1; j <= n && wl > 0; j++) {
            fx_mul(w, &wl, 5);
            fx_div(w, &wl, (uint32_t)(2 * j));
        }
        for (k = 0; wl > 0; k++) {
            if (k > 0) {
                fx_mul(w, &wl, 25);
                fx_div2(w, &wl, 4 * k, n + k);
                if (wl == 0) break;
            }
            fx_add(I, &Il, w, wl);
            for (j = 1; j <= k; j++) {
                memcpy(q, w, sizeof q); ql = wl;
                fx_div(q, &ql, (uint32_t)j);
                fx_add(H, &Hl, q, ql);
            }
            for (j = 1; j <= n + k; j++) {
                memcpy(q, w, sizeof q); ql = wl;
                fx_div(q, &ql, (uint32_t)j);
                fx_add(H, &Hl, q, ql);
            }
        }
        fx_div(H, &Hl, 2);

        /* pr = c * I (then drop FB fraction limbs) */
        for (u = 0; u < cl; u++) {
            uint64_t carry = 0;
            for (v = 0; v < Il; v++) {
                uint64_t tt = (uint64_t)c[u] * I[v] + pr[u + v] + carry;
                pr[u + v] = (uint32_t)tt;
                carry = tt >> 32;
            }
            pr[u + Il] = (uint32_t)carry;
        }
        pl = cl + Il;
        fx_norm(pr, &pl);
        pl = pl > FB ? pl - FB : 0;

        if (n & 1) {
            fx_add(P, &Pl, pr + FB, pl);
            fx_add(N, &Nl, H, Hl);
        } else {
            fx_add(N, &Nl, pr + FB, pl);
            fx_add(P, &Pl, H, Hl);
        }
    }

    if (fx_cmp(P, Pl, N, Nl) > 0) {
        fx_sub(P, &Pl, N, Nl);
        memset(t, 0, cap * sizeof(uint32_t));
        t[FB - 1] = 0x80000000u;
        fx_add(P, &Pl, t, FB);
        for (i = Pl; i > FB; i--) {
            r = r << 8; r = r << 8; r = r << 8; r = r << 8;
            r += (uint64_t)P[i - 1];
        }
    }

    free(t); free(P); free(N);
    return r;
}
