/*
 * A005950  Theta series of 18-dimensional lattice associated with Sp_4(4),
 *          with det 256 and minimal norm 3.
 * https://oeis.org/A005950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [q^n] ( theta^18 - 36*theta^14*F + 396*theta^10*F^2 -
 *            960*theta^6*F^3 ), where theta = Sum_{k in Z} q^(k^2) and F =
 *            Sum_{m odd} sigma(m) q^m = q*psi^4 with psi = Sum_{k>=0}
 *            q^(k(k+1)); i.e. the theta series lies in M_9(Gamma0(4),
 *            chi_{-4})
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *G[4];
static idx_t cap = 0;

static void mul_theta(seq_t *dst, const seq_t *src, idx_t N)
{
    idx_t i, k;
    for (i = 0; i < N; i++) {
        seq_t t = 0;
        for (k = 1; k * k <= i; k++)
            t += src[i - k * k];
        dst[i] = src[i] + t + t;
    }
}

static void mul_psi(seq_t *dst, const seq_t *src, idx_t N)
{
    idx_t i, k;
    for (i = 0; i < N; i++) {
        seq_t s = 0;
        for (k = 0; k * (k + 1) <= i; k++)
            s += src[i - k * (k + 1)];
        dst[i] = s;
    }
}

static void apply(seq_t **cur, seq_t **tmp, int kind, int times, idx_t N)
{
    int j;
    for (j = 0; j < times; j++) {
        seq_t *sw;
        if (kind == 0) mul_theta(*tmp, *cur, N);
        else mul_psi(*tmp, *cur, N);
        sw = *cur; *cur = *tmp; *tmp = sw;
    }
}

static seq_t *copy_arr(const seq_t *src, idx_t N)
{
    seq_t *c = alloc_zero(N);
    idx_t i;
    for (i = 0; i < N; i++) c[i] = src[i];
    return c;
}

static void build(idx_t N)
{
    seq_t *cur = alloc_zero(N);
    seq_t *tmp = alloc_zero(N);
    seq_t *w, *wt;

    cur[0] = 1;
    apply(&cur, &tmp, 0, 6, N);          /* theta^6 */

    w = copy_arr(cur, N); wt = alloc_zero(N);
    apply(&w, &wt, 1, 12, N);            /* theta^6 psi^12 */
    release(wt, N); G[3] = w;

    apply(&cur, &tmp, 0, 4, N);          /* theta^10 */
    w = copy_arr(cur, N); wt = alloc_zero(N);
    apply(&w, &wt, 1, 8, N);             /* theta^10 psi^8 */
    release(wt, N); G[2] = w;

    apply(&cur, &tmp, 0, 4, N);          /* theta^14 */
    w = copy_arr(cur, N); wt = alloc_zero(N);
    apply(&w, &wt, 1, 4, N);             /* theta^14 psi^4 */
    release(wt, N); G[1] = w;

    apply(&cur, &tmp, 0, 4, N);          /* theta^18 */
    G[0] = cur;
    release(tmp, N);
}

seq_t a(idx_t n)
{
    seq_t pos, neg;
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        int j;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        if (cap > 0)
            for (j = 0; j < 4; j++) release(G[j], cap);
        build(nc);
        cap = nc;
    }
    pos = G[0][n];
    if (n >= 2) pos += G[2][n - 2] * 396;
    neg = 0;
    if (n >= 1) neg += G[1][n - 1] * 36;
    if (n >= 3) neg += G[3][n - 3] * 960;
    return pos - neg;
}
