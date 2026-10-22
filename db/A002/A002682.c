/*
 * A002682  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ((n+1)/2)*M(n) + (2n+2)*M(n+1), where M(n)
 *            = (2/(2n+1)!) * Integral_{t=0..1} t*Product_{k=1..n} (t^2 - k^2)
 *            = (1/(2n+1)!) * Integral_{u=0..1} Product_{k=1..n} (u - k^2) du.
 *            With F = (n+2)! and g_j = e_j*F/(j+1), where e_j are the
 *            coefficients of Product_{k}(u + k^2), A_m = S...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   z2682_add, z2682_alt, z2682_bitlen, z2682_cmp, z2682_divs,
 *            z2682_iszero, z2682_leg, z2682_mods, z2682_muls, z2682_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/z2682_add.h"
#include "../utils/z2682_alt.h"
#include "../utils/z2682_bitlen.h"
#include "../utils/z2682_cmp.h"
#include "../utils/z2682_divs.h"
#include "../utils/z2682_iszero.h"
#include "../utils/z2682_leg.h"
#include "../utils/z2682_mods.h"
#include "../utils/z2682_muls.h"
#include "../utils/z2682_sub.h"

seq_t a(idx_t n) {
    idx_t m = n + 1, k, j, i;
    size_t bits = 256, fbits = 64, W, cur;
    uint32_t *g, *tmp, *An, *An1, *X;
    uint64_t lim, p;
    unsigned char *comp;
    seq_t den = 1;

    for (k = 1; k <= m; k++) bits += 2 * z2682_bitlen(k) + 1;
    for (i = 1; i <= n + 2; i++) { bits += z2682_bitlen(i); fbits += z2682_bitlen(i); }
    W = bits / 32 + 2;

    g = (uint32_t *)calloc((size_t)(m + 1) * W, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(W, sizeof(uint32_t));
    An = (uint32_t *)calloc(W, sizeof(uint32_t));
    An1 = (uint32_t *)calloc(W, sizeof(uint32_t));
    X = (uint32_t *)calloc(W, sizeof(uint32_t));

    /* g_0 = F = (n+2)! */
    g[0] = 1;
    for (i = 2; i <= n + 2; i++) z2682_muls(g, W, (uint32_t)i);
    cur = fbits / 32 + 2;
    if (cur > W) cur = W;

    for (k = 0; k <= m; k++) {
        if (k > 0) {
            fbits += 2 * z2682_bitlen(k) + 1;
            cur = fbits / 32 + 2;
            if (cur > W) cur = W;
            for (j = k; j >= 1; j--) {
                uint32_t *gj = g + (size_t)j * W;
                memcpy(tmp, g + (size_t)(j - 1) * W, cur * sizeof(uint32_t));
                z2682_muls(tmp, cur, (uint32_t)j);
                z2682_divs(tmp, cur, (uint32_t)(j + 1));
                z2682_muls(gj, cur, (uint32_t)k);
                z2682_muls(gj, cur, (uint32_t)k);
                z2682_add(gj, tmp, cur);
            }
            z2682_muls(g, cur, (uint32_t)k);
            z2682_muls(g, cur, (uint32_t)k);
        }
        if (k == n) z2682_alt(g, k, W, An, tmp);
        if (k == m) z2682_alt(g, k, W, An1, tmp);
    }

    /* X = |(2n+2)(2n+3) A_n - 4 A_{n+1}| */
    z2682_muls(An, W, (uint32_t)(2 * n + 2));
    z2682_muls(An, W, (uint32_t)(2 * n + 3));
    z2682_muls(An1, W, 4);
    if (z2682_cmp(An, An1, W) >= 0) {
        memcpy(X, An, W * sizeof(uint32_t));
        z2682_sub(X, An1, W);
    } else {
        memcpy(X, An1, W * sizeof(uint32_t));
        z2682_sub(X, An, W);
    }

    if (!z2682_iszero(X, W)) {
        lim = 2 * (uint64_t)n + 3;
        comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        for (p = 2; p <= lim; p++) {
            uint64_t vD, vN = 0, e;
            if (comp[p]) continue;
            for (e = p * p; e <= lim; e += p) comp[e] = 1;
            vD = (p == 2 ? 1 : 0) + z2682_leg(lim, p) + z2682_leg((uint64_t)n + 2, p);
            {
                uint64_t t = (uint64_t)n + 1;
                while (t % p == 0) { vN++; t /= p; }
            }
            if (vN < vD) {
                memcpy(tmp, X, W * sizeof(uint32_t));
                while (vN < vD && z2682_mods(tmp, W, (uint32_t)p) == 0) {
                    z2682_divs(tmp, W, (uint32_t)p);
                    vN++;
                }
            }
            for (e = vN; e < vD; e++) den *= (seq_t)p;
        }
        free(comp);
    }

    free(g); free(tmp); free(An); free(An1); free(X);
    return den;
}
