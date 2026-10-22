/*
 * A000242  3rd power of rooted tree enumerator; number of linear forests of 3
 *          rooted trees.
 * https://oeis.org/A000242
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [x^n] B(x)^3, where B(x) = Sum_{k>=1} r(k) x^k is the
 *            g.f. of A000081: r(1) = 1, r(m+1) = (1/m) * Sum_{k=1..m} b(k) *
 *            r(m+1-k), with b(k) = Sum_{d|k} d*r(d)
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 87,
 *            big any n
 * Helpers:   alloc_series
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_series.h"

static seq_t *R = NULL;   /* R[k]  = r(k), A000081 */
static seq_t *Bv = NULL;  /* Bv[k] = b(k) = Sum_{d|k} d*r(d) */
static seq_t *B2 = NULL;  /* B2[k] = [x^k] B(x)^2 */
static size_t cap = 0;    /* allocated length of each array */
static size_t nr = 0;     /* r(1..nr) known */
static size_t nb2 = 0;    /* B2[0..nb2-1] known */
static int bounded = -1;

static void grow(size_t need)
{
    size_t nc, i;
    seq_t *r2, *b2, *q2;
    if (need <= cap) return;
    nc = cap ? cap : 64;
    while (nc < need) nc *= 2;
    r2 = alloc_series(nc);
    b2 = alloc_series(nc);
    q2 = alloc_series(nc);
    for (i = 0; i < nc; i++) {
        if (i < cap) { r2[i] = R[i]; b2[i] = Bv[i]; q2[i] = B2[i]; }
        else { r2[i] = (seq_t)0; b2[i] = (seq_t)0; q2[i] = (seq_t)0; }
    }
    R = r2; Bv = b2; B2 = q2;
    cap = nc;
}

static void compute_b(size_t k)
{
    seq_t s = (seq_t)0;
    size_t d;
    for (d = 1; d * d <= k; d++) {
        if (k % d == 0) {
            size_t e = k / d;
            s += (seq_t)d * R[d];
            if (e != d) s += (seq_t)e * R[e];
        }
    }
    Bv[k] = s;
}

/* compute r(m+1) given r(1..m) */
static void next_r(size_t m)
{
    seq_t dm = (seq_t)m;
    if (dm == (seq_t)0) { R[m + 1] = (seq_t)0; return; }
    if (bounded && m <= 200) {
        /* overflow-safe: divide each term by m via quotient/remainder splitting,
           so no intermediate exceeds r(m+1) (plus tiny remainders) */
        seq_t q = (seq_t)0, rem = (seq_t)0;
        size_t d, j;
        for (d = 1; d <= m; d++) {
            seq_t c = (seq_t)0, u, uq, ur, rq, rr;
            for (j = d; j <= m; j += d) c += R[m + 1 - j];
            u = (seq_t)d * c;
            uq = u / dm; ur = u % dm;
            rq = R[d] / dm; rr = R[d] % dm;
            q += uq * R[d];
            q += ur * rq;
            rem += ur * rr;
            q += rem / dm;
            rem = rem % dm;
        }
        R[m + 1] = q;
    } else {
        seq_t S = (seq_t)0;
        size_t k;
        for (k = 1; k <= m; k++) S += Bv[k] * R[m + 1 - k];
        R[m + 1] = S / dm;
    }
}

seq_t a(idx_t n)
{
    size_t N, i, m;
    seq_t s;
    if (n < 3) return (seq_t)0;
    if (bounded < 0) {
        seq_t z = (seq_t)0;
        z -= 1;
        bounded = (z > (seq_t)0) ? 1 : 0;
    }
    N = (size_t)n;
    grow(N + 2);
    if (nr == 0) {
        R[1] = (seq_t)1;
        compute_b(1);
        nr = 1;
    }
    while (nr < N) {
        next_r(nr);
        nr++;
        compute_b(nr);
    }
    if (nb2 == 0) {
        B2[0] = (seq_t)0;
        B2[1] = (seq_t)0;
        nb2 = 2;
    }
    while (nb2 <= N) {
        m = nb2;
        s = (seq_t)0;
        for (i = 1; i + 1 <= m; i++) s += R[i] * R[m - i];
        B2[m] = s;
        nb2++;
    }
    s = (seq_t)0;
    for (i = 1; i + 2 <= N; i++) s += R[i] * B2[N - i];
    return s;
}
