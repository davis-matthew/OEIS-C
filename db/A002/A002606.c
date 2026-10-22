/*
 * A002606  Weight distribution of Karlin's [28,14,8] double circulant code.
 * https://oeis.org/A002606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of t^n in W(t) = Sum_{j=0..3} c_j
 *            (1+t)^(14-4j) (t(1-t)^2)^j, the Gleason form for an even
 *            formally self-dual [28,14,8] code (weights 2n); c_j fixed by
 *            a(0)=1, a(1)=a(2)=a(3)=0; a(n) = 0 for n > 14
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   a2606_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a2606_mul.h"

seq_t a(idx_t n)
{
    static const long long onep[2] = {1, 1};     /* 1 + t */
    static const long long g[4] = {0, 1, -2, 1}; /* t(1-t)^2 */
    long long P[4][15], c[4], w;
    int j, i, k;

    if (n > 14) return 0;

    for (j = 0; j < 4; j++) {
        for (i = 0; i < 15; i++) P[j][i] = 0;
        P[j][0] = 1;
        for (k = 0; k < 14 - 4 * j; k++) a2606_mul(P[j], onep, 2);
        for (k = 0; k < j; k++) a2606_mul(P[j], g, 4);
    }

    /* P[j] has lowest term t^j with coefficient 1: triangular solve */
    for (j = 0; j < 4; j++) {
        long long s = (j == 0) ? 1 : 0;
        for (k = 0; k < j; k++) s -= c[k] * P[k][j];
        c[j] = s;
    }

    w = 0;
    for (j = 0; j < 4; j++) w += c[j] * P[j][(int)n];
    return (seq_t)(unsigned long long)w;
}
