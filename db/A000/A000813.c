/*
 * A000813  Expansion of (sin x + cos x)/cos 4x.
 * https://oeis.org/A000813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. (sin x + cos x)/cos 4x = (sum_j c(j) x^j/j!) * (sum_k
 *            16^k E(2k) x^(2k)/(2k)!), so a(n) = Sum_{m=0..n, m even}
 *            binomial(n,n-m) * c(n-m) * 4^m * A000111(m), where c(j) =
 *            (-1)^floor(j/2) and A000111 comes from the Seidel boustrophedon:
 *            E(0,0) = 1, E(m,0) = 0, E(m,k) = E(m,k-1) + E(m-1,m-k),...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *zz = 0;    /* zigzag numbers A000111(0..have-1) */
static seq_t *row = 0;   /* last boustrophedon row */
static seq_t *tmp = 0;
static seq_t *bin = 0;   /* Pascal row */
static idx_t have = 0, cap = 0;

static seq_t *grow_to(seq_t *p, idx_t oldlen, idx_t newlen)
{
    if (p == 0) return a306_alloc0(newlen);
    return a306_grow(p, oldlen, newlen);
}

static void ensure(idx_t n)
{
    idx_t m, k;
    if (cap < n + 1) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        zz = grow_to(zz, cap, nc);
        row = grow_to(row, cap, nc);
        tmp = grow_to(tmp, cap, nc);
        bin = grow_to(bin, cap, nc);
        cap = nc;
    }
    for (m = have; m <= n; m++) {
        if (m == 0) {
            row[0] = 1;
            zz[0] = 1;
        } else {
            seq_t *t;
            tmp[0] = 0;
            for (k = 1; k <= m; k++) tmp[k] = tmp[k - 1] + row[m - k];
            t = row; row = tmp; tmp = t;
            zz[m] = row[m];
        }
    }
    if (have < n + 1) have = n + 1;
}

seq_t a(idx_t n)
{
    idx_t i, k, m;
    seq_t pos = 0, neg = 0, pw = 1;
    ensure(n);
    bin[0] = 1;
    for (i = 1; i <= n; i++) {
        bin[i] = 0;
        for (k = i; k >= 1; k--) bin[k] += bin[k - 1];
    }
    for (m = 0; m <= n; m += 2) {
        idx_t j = n - m;
        seq_t term = bin[j] * pw * zz[m];
        if ((j & 2) == 0) pos += term;
        else neg += term;
        pw *= 16;
    }
    return pos - neg;
}
