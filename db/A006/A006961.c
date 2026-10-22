/*
 * A006961  Number of mappings from n points to themselves with in-degree <=
 *          2.
 * https://oeis.org/A006961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: 1/Prod_{k>=1} (1 - T(x^k)), where T(x) = Sum_{n>=1}
 *            A001190(n) x^n, A001190(1) = 1, A001190(2m+1) = Sum_{i=1..m}
 *            A001190(i)*A001190(2m+1-i), A001190(2m) = Sum_{i=1..m-1}
 *            A001190(i)*A001190(2m-i) + A001190(m)*(A001190(m)+1)/2. Each
 *            factor 1/(1 - T(x^k)) is applied in place: g[i] = f[i] + Sum...
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *W = NULL;   /* Wedderburn-Etherington numbers A001190 */
static seq_t *F = NULL;   /* terms of this sequence */
static idx_t cap = 0;

static void build(idx_t newcap) {
    idx_t i, j, k;
    W = grow_zero(W, cap, newcap);
    F = grow_zero(F, cap, newcap);

    /* extend W (W[0] = 0, W[1] = 1) */
    for (i = (cap < 1 ? 1 : cap); i < newcap; i++) {
        seq_t s = 0;
        if (i == 1) {
            W[1] = 1;
            continue;
        }
        for (j = 1; 2 * j < i; j++)
            s += W[j] * W[i - j];
        if (i % 2 == 0) {
            seq_t h = W[i / 2];
            if (h % 2 == 0)
                s += (h / 2) * (h + 1);
            else
                s += h * ((h + 1) / 2);
        }
        W[i] = s;
    }
    cap = newcap;

    /* recompute F from scratch */
    for (i = 0; i < cap; i++)
        F[i] = 0;
    F[0] = 1;
    for (k = 1; k < cap; k++) {
        for (i = k; i < cap; i++) {
            seq_t s = 0;
            for (j = 1; j * k <= i; j++)
                s += W[j] * F[i - j * k];
            F[i] += s;
        }
    }
}

seq_t a(idx_t n) {
    if (n >= cap) {
        idx_t nc = 2 * cap;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        build(nc);
    }
    return F[n];
}
