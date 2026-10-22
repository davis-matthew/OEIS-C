/*
 * A000784  Number of symmetrical planar partitions of n (planar partitions
 *          (A000219) that when regarded as 3-D objects have just one symmetry
 *          plane).
 * https://oeis.org/A000784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A005987(n) - A048141(n): symmetric plane partitions minus
 *            totally symmetric ones. A005987 has g.f. Product_{k odd}
 *            1/(1-x^k) * Product_{m>=1} 1/(1-x^(2m))^floor(m/2); A048141
 *            (totally symmetric plane partitions of n) is counted by an
 *            exhaustive search over x-slices of the fundamental regio...
 * Range:     u8 n <= 18, u16 n <= 43, u32 n <= 109, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *Sv = 0;
static uint64_t *Tc = 0;
static long Ncap = -1;
static long LIM;
static int *buf = 0;

static void go(int x, int *r, int nr, long W);

static void rows(int x, const int *r, int nr, int *nw, int z, long W)
{
    int X = x + 1;
    int rz, up, e;
    if (z > 0) go(X, nw, z, W);
    rz = z < nr ? r[z] : z;
    up = (rz == X) ? X + 1 : rz;
    if (z > 0 && nw[z - 1] < up) up = nw[z - 1];
    for (e = z + 1; e <= up; e++) {
        long wt;
        if (z == X) wt = 1;
        else {
            int len = e - z;
            if (len == 1) wt = 3;
            else if (e == X + 1) wt = 6 + 6L * (len - 2);
            else wt = 3 + 6L * (len - 1);
        }
        if (W + wt > LIM) break;
        nw[z] = e;
        rows(x, r, nr, nw, z + 1, W + wt);
    }
}

static void go(int x, int *r, int nr, long W)
{
    Tc[W]++;
    rows(x, r, nr, r + nr, 0, W);
}

static void compute(long N)
{
    long i, d, m, k;
    if (Sv) release(Sv, (idx_t)(Ncap + 1));
    free(Tc);
    free(buf);
    Sv = alloc_zero((idx_t)(N + 1));
    Tc = (uint64_t *)calloc((size_t)(N + 1), sizeof(uint64_t));
    buf = (int *)malloc(sizeof(int) * (size_t)(3 * N + 64));

    /* symmetric plane partitions */
    Sv[0] = 1;
    for (d = 1; d <= N; d += 2)
        for (i = d; i <= N; i++) Sv[i] += Sv[i - d];
    for (m = 2; 2 * m <= N; m++) {
        d = 2 * m;
        for (k = 0; k < m / 2; k++)
            for (i = d; i <= N; i++) Sv[i] += Sv[i - d];
    }

    /* totally symmetric plane partitions */
    LIM = N;
    Tc[0] = 1;
    if (N >= 1) {
        buf[0] = 1;
        go(0, buf, 1, 1);
    }
    Ncap = N;
}

seq_t a(idx_t n)
{
    if ((long)n > Ncap) {
        long N = Ncap + Ncap / 20 + 8;
        if (N < (long)n) N = (long)n;
        compute(N);
    }
    return Sv[n] - (seq_t)Tc[n];
}
