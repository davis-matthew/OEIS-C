/*
 * A005952  Theta series of P_{12a} packing.
 * https://oeis.org/A005952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = [q^(2n)] of sum_w A_w * E(q)^(12-w) * O(q)^w, where E(q)
 *            = Sum_{k even} q^(k^2), O(q) = Sum_{k odd} q^(k^2), and
 *            (A_0,A_4,A_6,A_8,A_12) = (1,51,40,51,1) is the distance
 *            distribution of the nonlinear (12,144,4) code (P_{12a} =
 *            Construction A applied to that code). Evaluated with
 *            homogeneous...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cachelen = 0;

static void build(idx_t m)
{
    static const int cw[13] = {1, 0, 0, 0, 51, 0, 40, 0, 51, 0, 0, 0, 1};
    idx_t N = 2 * (m - 1);
    idx_t L = N + 1;
    seq_t *g = alloc_zero(L);
    seq_t *op = alloc_zero(L);
    seq_t *tmp = alloc_zero(L);
    idx_t i, e, k, s;
    g[0] = 1;
    op[0] = 1;
    for (k = 1; k <= 12; k++) {
        /* op = op * O */
        for (e = 0; e < L; e++) tmp[e] = 0;
        for (e = 0; e < L; e++) {
            if (op[e] == 0) continue;
            for (s = 1; e + s * s <= N; s += 2)
                tmp[e + s * s] += 2 * op[e];
        }
        {
            seq_t *t = op; op = tmp; tmp = t;
        }
        /* g = g * E, in place, descending */
        for (i = L; i-- > 0;) {
            if (g[i] == 0) continue;
            for (s = 1; i + 4 * s * s <= N; s++)
                g[i + 4 * s * s] += 2 * g[i];
        }
        if (cw[k] != 0) {
            for (e = 0; e < L; e++)
                if (op[e] != 0) g[e] += op[e] * cw[k];
        }
    }
    if (cache) release(cache, cachelen);
    cache = alloc_zero(m);
    for (i = 0; i < m; i++) cache[i] = g[2 * i];
    cachelen = m;
    release(g, L);
    release(op, L);
    release(tmp, L);
}

seq_t a(idx_t n)
{
    if (n >= cachelen) {
        idx_t m = n + 1;
        if (m < 2 * cachelen) m = 2 * cachelen;
        if (m < 64) m = 64;
        build(m);
    }
    return cache[n];
}
