/*
 * A002861  Number of connected functions (or mapping patterns) on n unlabeled
 *          points, or number of rings and branches with n edges.
 * https://oeis.org/A002861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(n/d) * c(d) for n >= 1, a(0) = 0,
 *            where c(m) = m*r(m) + Sum_{k=1..m-1} r(k)*c(m-k) is m*[x^m]
 *            log(1/(1-R(x))), R(x) = Sum r(m) x^m is the g.f. of A000081 (CIK
 *            transform of A000081), r(1) = 1, r(m+1) = (1/m) * Sum_{k=1..m}
 *            (Sum_{d|k} d*r(d)) * r(m-k+1)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Helpers:   a001028_grow, a001028_z, a2861_is_fixed, a2861_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a001028_grow.h"
#include "../utils/a001028_z.h"
#include "../utils/a2861_is_fixed.h"
#include "../utils/a2861_phi.h"

typedef unsigned __int128 a2861_u128;

/* exact division S/n modulo 2^128 (correct when the quotient fits in 128 - v2(n) bits) */
static a2861_u128 a2861_exdiv(a2861_u128 S, idx_t n)
{
    int v = 0, i;
    idx_t o = n;
    a2861_u128 inv, q;
    while ((o & 1) == 0) { o >>= 1; v++; }
    inv = (a2861_u128)o;
    for (i = 0; i < 8; i++) inv *= (a2861_u128)2 - (a2861_u128)o * inv;
    q = (S >> v) * inv;
    if (v > 0) q &= (((a2861_u128)1) << (128 - v)) - 1;
    return q;
}

/* fixed-width state */
static a2861_u128 *a2861_fr = NULL, *a2861_fs = NULL, *a2861_fc = NULL;
static idx_t a2861_fN = 0, a2861_fcap = 0;

static void a2861_fixed_ensure(idx_t n)
{
    idx_t m, k, d;
    if (n + 1 > a2861_fcap) {
        idx_t nc = (n + 1) * 2;
        a2861_fr = (a2861_u128 *)realloc(a2861_fr, nc * sizeof(a2861_u128));
        a2861_fs = (a2861_u128 *)realloc(a2861_fs, nc * sizeof(a2861_u128));
        a2861_fc = (a2861_u128 *)realloc(a2861_fc, nc * sizeof(a2861_u128));
        a2861_fcap = nc;
    }
    for (m = a2861_fN + 1; m <= n; m++) {
        a2861_u128 S, t;
        if (m == 1) {
            a2861_fr[1] = 1;
        } else {
            S = 0;
            for (k = 1; k < m; k++) S += a2861_fs[k] * a2861_fr[m - k];
            a2861_fr[m] = a2861_exdiv(S, m - 1);
        }
        t = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                t += (a2861_u128)d * a2861_fr[d];
                if (d != m / d) t += (a2861_u128)(m / d) * a2861_fr[m / d];
            }
        }
        a2861_fs[m] = t;
        S = (a2861_u128)m * a2861_fr[m];
        for (k = 1; k < m; k++) S += a2861_fr[k] * a2861_fc[m - k];
        a2861_fc[m] = S;
    }
    if (n > a2861_fN) a2861_fN = n;
}

/* arbitrary-precision state */
static seq_t *a2861_br = NULL, *a2861_bs = NULL, *a2861_bc = NULL;
static idx_t a2861_bN = 0, a2861_bcap = 0;

static void a2861_big_ensure(idx_t n)
{
    idx_t m, k, d;
    if (n + 1 > a2861_bcap) {
        idx_t nc = (n + 1) * 2;
        if (a2861_bcap == 0) {
            a2861_br = a001028_z(nc);
            a2861_bs = a001028_z(nc);
            a2861_bc = a001028_z(nc);
        } else {
            a2861_br = a001028_grow(a2861_br, a2861_bcap, nc);
            a2861_bs = a001028_grow(a2861_bs, a2861_bcap, nc);
            a2861_bc = a001028_grow(a2861_bc, a2861_bcap, nc);
        }
        a2861_bcap = nc;
    }
    for (m = a2861_bN + 1; m <= n; m++) {
        seq_t S, t;
        if (m == 1) {
            a2861_br[1] = 1;
        } else {
            S = 0;
            for (k = 1; k < m; k++) S += a2861_bs[k] * a2861_br[m - k];
            a2861_br[m] = S / (seq_t)(m - 1);
        }
        t = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                t += (seq_t)d * a2861_br[d];
                if (d != m / d) t += (seq_t)(m / d) * a2861_br[m / d];
            }
        }
        a2861_bs[m] = t;
        S = (seq_t)m * a2861_br[m];
        for (k = 1; k < m; k++) S += a2861_br[k] * a2861_bc[m - k];
        a2861_bc[m] = S;
    }
    if (n > a2861_bN) a2861_bN = n;
}

seq_t a(idx_t n)
{
    idx_t d;
    if (n == 0) return 0;
    if (a2861_is_fixed()) {
        a2861_u128 S = 0;
        a2861_fixed_ensure(n);
        for (d = 1; d <= n; d++)
            if (n % d == 0) S += (a2861_u128)a2861_phi(n / d) * a2861_fc[d];
        return (seq_t)a2861_exdiv(S, n);
    } else {
        seq_t S = 0;
        a2861_big_ensure(n);
        for (d = 1; d <= n; d++)
            if (n % d == 0) S += (seq_t)a2861_phi(n / d) * a2861_bc[d];
        return S / (seq_t)n;
    }
}
