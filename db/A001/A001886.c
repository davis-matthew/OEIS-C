/*
 * A001886  Hit polynomials.
 * https://oeis.org/A001886
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=3..n} (-1)^(j-3)*binomial(j,3)*r(n,j)*(n-j)!,
 *            where r(n,j) is the number of ways to place j non-attacking
 *            rooks on the n X n tridiagonal board (cells with |i-k| <= 1); so
 *            a(n) counts permutations p of [n] with exactly 3 values i such
 *            that |p(i)-i| <= 1. Rook polynomials: R_n(x) = (1...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    size_t L = (size_t)n + 2;
    seq_t *buf[4];
    for (int k = 0; k < 4; k++) {
        buf[k] = (seq_t *)malloc(L * sizeof(seq_t));
        for (size_t i = 0; i < L; i++)
            buf[k][i] = 0;
    }
    /* buf[m % 4] holds R_m; buf[3] (R_{-1}) stays zero initially */
    buf[0][0] = 1;
    buf[1][0] = 1;
    buf[1][1] = 1;
    for (idx_t m = 2; m <= n; m++) {
        seq_t *Rm = buf[m % 4];
        const seq_t *R1 = buf[(m + 3) % 4];
        const seq_t *R2 = buf[(m + 2) % 4];
        const seq_t *R3 = buf[(m + 1) % 4];
        for (idx_t j = 0; j <= m; j++) {
            seq_t v = R1[j];
            if (j >= 1) {
                v += R1[j - 1];
                v += R1[j - 1];
                v += R2[j - 1];
            }
            if (j >= 3)
                v -= R3[j - 3];
            Rm[j] = v;
        }
    }
    const seq_t *R = buf[n % 4];
    seq_t pos = 0, neg = 0, f = 1;
    for (idx_t j = n; j >= 3; j--) {
        idx_t c = j * (j - 1) / 2 * (j - 2) / 3;
        seq_t term = R[j] * f * (seq_t)c;
        if ((j - 3) & 1)
            neg += term;
        else
            pos += term;
        f *= (seq_t)(n - j + 1);
    }
    for (int k = 0; k < 4; k++)
        free(buf[k]);
    return pos - neg;
}
