/*
 * A000200  Number of bicentered hydrocarbons with n atoms.
 * https://oeis.org/A000200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   Count bicentered trees by height. A bicentered tree is two
 *            rooted trees of the same height h joined by an edge, and every
 *            node has at most 3 children (carbon valence 4). R_h(x) = x*(1 +
 *            R + Z(S2;R) + Z(S3;R)) with R = R_{h-1} and R_{-1} = 0 (Polya
 *            cycle indices; R_h counts these rooted trees of h...
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 53, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 56 (2026-11-03)
 * Helpers:   mem, triangular_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/triangular_seq.h"
#include "../utils/mem.h"

static seq_t a200_ch3(seq_t r)
{
    seq_t u = r, v = r + 1, w = r + 2;
    if (u % 3 == 0) u /= 3; else if (v % 3 == 0) v /= 3; else w /= 3;
    if (u % 2 == 0) u /= 2; else v /= 2;
    return u * v * w;
}

static seq_t *a200_zalloc(long long n)
{
    seq_t *p = mem_new(seq_t, (size_t)n);
    long long i;
    for (i = 0; i < n; i++) p[i] = 0;
    return p;
}

static seq_t *a200_memo = 0;
static long long a200_N = -1;

static void a200_build(long long N)
{
    long long S = N + 2, h, d, i, j;
    seq_t *R = a200_zalloc(S), *Z2 = a200_zalloc(S), *Z3 = a200_zalloc(S);
    seq_t *E = a200_zalloc(S), *M2 = a200_zalloc(S), *M3 = a200_zalloc(S);
    seq_t *A = a200_zalloc(S);

    for (h = 0; 2 * h + 2 <= N; h++) {
        long long L = N - h - 1;
        for (d = 0; d < S; d++) { E[d] = 0; M2[d] = 0; M3[d] = 0; }

        /* trees of height exactly h */
        for (d = h + 1; d <= L; d++) {
            seq_t v = R[d - 1];
            v += Z2[d - 1];
            v += Z3[d - 1];
            if (d == 1) v += 1;
            v -= R[d];
            E[d] = v;
        }

        /* M2 = Z(S2;E) up to N, M3 = Z(S3;E) up to L (knapsack over sizes) */
        for (i = h + 1; i <= L; i++) {
            seq_t r = E[i], c2r, c3r;
            long long hi;
            if (r == 0) continue;
            c2r = triangular_seq(r);
            c3r = a200_ch3(r);
            for (d = i + 2 * h + 2; d <= L; d++)
                M3[d] += M2[d - i] * r;
            hi = 3 * i - 1; if (hi > L) hi = L;
            for (d = 2 * i + h + 1; d <= hi; d++)
                M3[d] += E[d - 2 * i] * c2r;
            if (3 * i <= L) M3[3 * i] += c3r;
            hi = 2 * i - 1; if (hi > N) hi = N;
            for (d = i + h + 1; d <= hi; d++)
                M2[d] += E[d - i] * r;
            if (2 * i <= N) M2[2 * i] += c2r;
        }

        for (d = 2 * h + 2; d <= N; d++) A[d] += M2[d];

        /* Z3(R+E) = Z3(R) + Z2(R)E + R Z2(E) + Z3(E) */
        for (d = h + 1; d <= L; d++) {
            seq_t s = 0;
            for (i = h + 1; i <= d - 2; i++) s += E[i] * Z2[d - i];
            for (j = 2 * h + 2; j <= d - 1; j++) s += M2[j] * R[d - j];
            s += M3[d];
            Z3[d] += s;
        }
        /* Z2(R+E) = Z2(R) + R E + Z2(E) */
        for (d = h + 1; d <= L; d++) {
            seq_t s = 0;
            for (i = h + 1; i <= d - 1; i++) s += E[i] * R[d - i];
            s += M2[d];
            Z2[d] += s;
        }
        for (d = h + 1; d <= L; d++) R[d] += E[d];
    }

    if (a200_memo) mem_free(a200_memo);
    a200_memo = A;
    a200_N = N;
    mem_free(R); mem_free(Z2); mem_free(Z3); mem_free(E); mem_free(M2); mem_free(M3);
}

seq_t a(idx_t n)
{
    if ((long long)n > a200_N) {
        long long N = (long long)n;
        if (N < 2 * a200_N) N = 2 * a200_N;
        if (N < 32) N = 32;
        a200_build(N);
    }
    return a200_memo[(long long)n];
}
