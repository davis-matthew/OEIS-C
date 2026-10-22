/*
 * A003081  Number of triangular cacti with 2n+1 nodes (n triangles).
 * https://oeis.org/A003081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = b(2n+1), where B(x) = C(x) + (C(x^3) - C(x)^3)/3 and C(x)
 *            = x*exp(Sum_{k>=1} T(x^k)/k), T(x) = (C(x)^2 + C(x^2))/2 (C =
 *            g.f. of rooted triangular cacti, A003080(n) = c(2n+1))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 22, u64 n <= 40, u128 n <= 73,
 *            big any n
 * Helpers:   A_grow, a3081_fixed, a3081_inv, a3081_mc, a3081_mul, a3081_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A_grow.h"
#include "../utils/a3081_fixed.h"
#include "../utils/a3081_inv.h"
#include "../utils/a3081_mc.h"
#include "../utils/a3081_mul.h"
#include "../utils/a3081_w.h"

static seq_t *BU = NULL, *BT = NULL, *BS2 = NULL, *BSG = NULL;
static idx_t bhave = 0, bcap = 0;

static void a3081_ensure(idx_t n)
{
    idx_t m, i, d;
    if (n < bhave) return;
    if (n + 1 > bcap) {
        idx_t nc = 2 * bcap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 16) nc = 16;
        BU = A_grow(BU, bcap, nc);
        BT = A_grow(BT, bcap, nc);
        BS2 = A_grow(BS2, bcap, nc);
        BSG = A_grow(BSG, bcap, nc);
        bcap = nc;
    }
    for (m = bhave; m <= n; m++) {
        if (m == 0) {
            BU[0] = 1; BT[0] = 0; BSG[0] = 0; BS2[0] = 1;
            continue;
        }
        {
            seq_t t = BS2[m - 1];
            if ((m - 1) % 2 == 0) t += BU[(m - 1) / 2];
            BT[m] = t / 2;
        }
        {
            seq_t sg = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    sg += BT[d] * d;
                    if (e != d) sg += BT[e] * e;
                }
            }
            BSG[m] = sg;
        }
        {
            seq_t g = 0;
            for (i = 1; i <= m; i++) g += BSG[i] * BU[m - i];
            BU[m] = g / m;
        }
        {
            seq_t s = 0;
            for (i = 0; 2 * i < m; i++) s += BU[i] * BU[m - i];
            s = s * 2;
            if (m % 2 == 0) s += BU[m / 2] * BU[m / 2];
            BS2[m] = s;
        }
    }
    bhave = n + 1;
}

seq_t a(idx_t n)
{
    int w = a3081_w();
    seq_t cube = 0, D = 0, E;
    idx_t i;
    if (w < 300) return a3081_fixed(n, w);
    a3081_ensure(n);
    for (i = 0; i < n; i++) cube += BU[i] * BS2[n - 1 - i];
    if ((2 * n + 1) % 3 == 0) D = BU[((2 * n + 1) / 3 - 1) / 2];
    E = cube - D;
    return BU[n] - E / 3;
}
