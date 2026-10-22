/*
 * A002875  Number of partitions of {1..3n} that are invariant under a
 *          permutation consisting of n 3-cycles with k orbits of blocks,
 *          maximized over k.
 * https://oeis.org/A002875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_k T(n,k), where T(n,k) = Sum_{i+j=k} U(n,i,j) and
 *            U(n,i,j) = (i+3j)*U(n-1,i,j) + U(n-1,i-1,j) + U(n-1,i,j-1),
 *            U(0,0,0) = 1. The n 3-cycles are grouped into i "fixed" groups
 *            (each one invariant block) and j "triple" groups (an orbit of 3
 *            blocks; a group of m cycles has 3^(m-1) choices)....
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n + 1;
    seq_t *U = (seq_t *)calloc(N * N, sizeof(seq_t));
    seq_t best, s;
    long long t, i, j, k;
    U[0] = 1;
    for (t = 1; t <= (long long)n; t++) {
        for (i = t; i >= 0; i--) {
            for (j = t - i; j >= 0; j--) {
                size_t idx = (size_t)i * N + (size_t)j;
                seq_t v = U[idx] * (seq_t)(i + 3 * j);
                if (i > 0) v += U[(size_t)(i - 1) * N + (size_t)j];
                if (j > 0) v += U[idx - 1];
                U[idx] = v;
            }
        }
    }
    best = 0;
    for (k = 0; k <= (long long)n; k++) {
        s = 0;
        for (i = 0; i <= k; i++)
            s += U[(size_t)i * N + (size_t)(k - i)];
        if (s > best) best = s;
    }
    free(U);
    return best;
}
