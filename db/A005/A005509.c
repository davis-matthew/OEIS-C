/*
 * A005509  Number of unrooted triangulations with reflection symmetry of a
 *          disk with 2 internal nodes and n+3 nodes on the boundary.
 * https://oeis.org/A005509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Let G_j(N) = [x^N] C(x)^j = binomial(2N+j-1,N) -
 *            binomial(2N+j-1,N-1) (C(x) = Catalan g.f.), and T(m) =
 *            binomial(2m-3,m-1) - binomial(2m-2,m-1) + binomial(2m-2,m)
 *            (triangulations of an m-gon with one interior node). Then n
 *            even, p = n/2+1: a(n) = 4*G_3(p-1) - 3*G_2(p-1) + T(p+1); n odd,
 *            p = (n+3)...
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 130,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

static void binoms(int cnt, const long long *M, const long long *K, seq_t *out)
{
    int i;
    if (seq_is_fixed_width()) {
        /* additive Pascal triangle: exact modulo 2^w, so exact whenever result fits */
        long long mx = 0, r, c;
        seq_t *row;
        for (i = 0; i < cnt; i++) {
            out[i] = 0;
            if (M[i] > mx) mx = M[i];
        }
        row = (seq_t *)malloc(sizeof(seq_t) * (size_t)(mx + 1));
        row[0] = 1;
        for (r = 0; r <= mx; r++) {
            if (r > 0) {
                row[r] = 1;
                for (c = r - 1; c >= 1; c--) row[c] += row[c - 1];
            }
            for (i = 0; i < cnt; i++)
                if (M[i] == r && K[i] >= 0 && K[i] <= r) out[i] = row[K[i]];
        }
        free(row);
    } else {
        for (i = 0; i < cnt; i++) {
            long long m = M[i], k = K[i], j;
            seq_t b = 1;
            if (k < 0 || k > m || m < 0) { out[i] = 0; continue; }
            if (k > m - k) k = m - k;
            for (j = 1; j <= k; j++)
                b = b * (seq_t)(idx_t)(m - k + j) / (seq_t)(idx_t)j;
            out[i] = b;
        }
    }
}

seq_t a(idx_t n)
{
    long long M[10], K[10];
    seq_t v[10];
    if (n % 2 == 0) {
        long long p = (long long)(n / 2) + 1, N = p - 1, m = p + 1;
        seq_t G3, G2, T;
        M[0] = 2 * N + 2; K[0] = N;
        M[1] = 2 * N + 2; K[1] = N - 1;
        M[2] = 2 * N + 1; K[2] = N;
        M[3] = 2 * N + 1; K[3] = N - 1;
        M[4] = 2 * m - 3; K[4] = m - 1;
        M[5] = 2 * m - 2; K[5] = m - 1;
        M[6] = 2 * m - 2; K[6] = m;
        binoms(7, M, K, v);
        G3 = v[0] - v[1];
        G2 = v[2] - v[3];
        T = v[4] + v[6] - v[5];
        return G3 * 4 + T - G2 * 3;
    } else {
        long long p = (long long)((n + 3) / 2), N = p - 2, N2 = p - 1, m = p + 1;
        seq_t G4, G3, G2, G1, T;
        M[0] = 2 * N + 3; K[0] = N;
        M[1] = 2 * N + 3; K[1] = N - 1;
        M[2] = 2 * N + 2; K[2] = N;
        M[3] = 2 * N + 2; K[3] = N - 1;
        M[4] = 2 * m - 3; K[4] = m - 1;
        M[5] = 2 * m - 2; K[5] = m - 1;
        M[6] = 2 * m - 2; K[6] = m;
        M[7] = 2 * N2 + 1; K[7] = N2;
        M[8] = 2 * N2 + 1; K[8] = N2 - 1;
        M[9] = 2 * N2; K[9] = N2 - 1;
        binoms(10, M, K, v);
        G4 = v[0] - v[1];
        G3 = v[2] - v[3];
        T = v[4] + v[6] - v[5];
        G2 = v[7] - v[8];
        G1 = v[5 - 5 + 9] ; /* placeholder replaced below */
        {
            long long Mc[1], Kc[1];
            seq_t c[1];
            Mc[0] = 2 * N2; Kc[0] = N2;
            binoms(1, Mc, Kc, c);
            G1 = c[0] - v[9];
        }
        return G4 * 4 + T + G2 - G3 * 2 - G1;
    }
}
