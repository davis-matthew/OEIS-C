/*
 * A002608  Glaisher's function T(2n+1).
 * https://oeis.org/A002608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (sigma_7(m) + 560*c(m))/17 with m = 2n+1, where c(m) is
 *            the coefficient of q^m in eta(q)^8*eta(q^2)^8 = q*Prod_{k>=1}
 *            (1-q^k)^8*(1-q^(2k))^8 (the weight-8 level-2 newform, c(1)=1,
 *            c(3)=12, c(5)=-210, c(7)=1016)
 * Range:     i8 n <= 0, i16 n <= 2, i32 n <= 15, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   a002608_mul_euler
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a002608_mul_euler.h"

static long long *a002608_F = NULL;   /* coefficients of Prod (1-q^k)^8 (1-q^2k)^8 */
static long long a002608_len = 0;

static void a002608_build(long long L)
{
    long long i;
    int t;
    long long *F = (long long *)malloc((size_t)L * sizeof(long long));
    for (i = 0; i < L; i++) F[i] = 0;
    F[0] = 1;
    for (t = 0; t < 8; t++) {
        a002608_mul_euler(F, L, 1);
        a002608_mul_euler(F, L, 2);
    }
    free(a002608_F);
    a002608_F = F;
    a002608_len = L;
}

seq_t a(idx_t n)
{
    long long m = 2 * (long long)n + 1;
    long long c, d, rs = 0;
    seq_t qs = 0;

    if (a002608_len < m) {
        long long L = 2 * a002608_len;
        if (L < m) L = m;
        if (L < 64) L = 64;
        a002608_build(L);
    }
    c = a002608_F[m - 1];

    /* sigma_7(m) = 17*qs + rs, accumulating d^7 = 17*Q + R per divisor */
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            long long dv[2];
            int k, cnt = 1;
            dv[0] = d;
            if (d != m / d) { dv[1] = m / d; cnt = 2; }
            for (k = 0; k < cnt; k++) {
                seq_t Q = 0;
                long long R = 1;
                int e;
                for (e = 0; e < 7; e++) {
                    long long tt = R * dv[k];
                    Q = Q * (seq_t)dv[k] + (seq_t)(tt / 17);
                    R = tt % 17;
                }
                qs += Q;
                rs += R;
            }
        }
    }
    /* 560*c = 17*32*c + 16*c */
    return qs + (seq_t)32 * (seq_t)c + (seq_t)((rs + 16 * c) / 17);
}
