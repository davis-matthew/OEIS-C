/*
 * A000137  Series-parallel numbers.
 * https://oeis.org/A000137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x(1+S)/(1-S), S = g.f. for A000084, with S(1) = 1 and S(n)
 *            = 2*b(n) for n >= 2, where b = A000669: b(1) = 1, b(n) = (1/n) *
 *            Sum_{j=1..n-1} j*b(j) * Sum_{i>=1, i*j<=n} c(n-i*j), c(0) = 1,
 *            c(1) = 1, c(n) = 2*b(n) for n >= 2 (Euler transform of b). With
 *            T = (1+S)/(1-S): T(0) = 1, T(m) = 2*S(m)...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_series.h"

static seq_t *Bv = 0, *Cv = 0, *Tv = 0;
static idx_t cap = 0;   /* allocated length */
static idx_t done = 0;  /* highest m with B, C, T computed */

static void grow(idx_t need)
{
    idx_t nc, i;
    seq_t *nb, *ncv, *nt;
    if (need < cap) return;
    nc = cap * 2;
    if (nc < need + 1) nc = need + 1;
    if (nc < 64) nc = 64;
    nb = alloc_series((size_t)nc);
    ncv = alloc_series((size_t)nc);
    nt = alloc_series((size_t)nc);
    for (i = 0; i < cap && i <= done; i++) {
        nb[i] = Bv[i];
        ncv[i] = Cv[i];
        nt[i] = Tv[i];
    }
    Bv = nb; Cv = ncv; Tv = nt;  /* old arrays are leaked */
    cap = nc;
}

static seq_t S_of(idx_t k)
{
    if (k == 1) return (seq_t)1;
    return Bv[k] * 2;
}

static void step(idx_t m)
{
    idx_t j, i, k;
    seq_t t;
    if (m == 1) {
        Bv[1] = (seq_t)1;
        Cv[1] = (seq_t)1;
    } else {
        seq_t nn = (seq_t)m;
        seq_t q = (seq_t)0, r = (seq_t)0;
        for (j = 1; j < m; j++) {
            seq_t Y = (seq_t)0, bq, br, yq, yr;
            for (i = j; i <= m; i += j) Y += Cv[m - i];
            bq = Bv[j] / nn; br = Bv[j] % nn;
            yq = Y / nn;     yr = Y % nn;
            q += (bq * Y + br * yq) * j;
            r += (br * yr) * j;
            q += r / nn;
            r %= nn;
        }
        Bv[m] = q;
        Cv[m] = q * 2;
    }
    t = S_of(m) * 2;
    for (k = 1; k < m; k++) t += S_of(k) * Tv[m - k];
    Tv[m] = t;
}

seq_t a(idx_t n)
{
    idx_t m;
    if (n < 1) return (seq_t)0;
    if (cap == 0) {
        grow(64);
        Bv[0] = (seq_t)0;
        Cv[0] = (seq_t)1;
        Tv[0] = (seq_t)1;
        done = 0;
    }
    if (n - 1 > done) {
        grow(n);
        for (m = done + 1; m <= n - 1; m++) {
            step(m);
            done = m;
        }
    }
    return Tv[n - 1];
}
