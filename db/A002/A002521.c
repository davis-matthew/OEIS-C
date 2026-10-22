/*
 * A002521  Weight distribution of [ 28,14,9 ] ternary self-dual code.
 * https://oeis.org/A002521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = [t^n] W(t), where by Gleason's theorem W(t) = (1+8t)^7 -
 *            56*(1+8t)^4*t*(1-t)^3 + 280*(1+8t)*t^2*(1-t)^6 (t = y^3, the
 *            weight enumerator of the extremal [28,14,9] ternary self-dual
 *            code; coefficients fixed by A_0=1, A_3=A_6=0); a(n)=0 for n>9
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   a002521_mul, a002521_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002521_mul.h"
#include "../utils/a002521_pow.h"

seq_t a(idx_t n)
{
    long long f[10], g[10], P1[10], P2[10], P3[10], T[10], W[10];
    int i;
    if (n > 9) return (seq_t)0;
    for (i = 0; i < 10; i++) { f[i] = 0; g[i] = 0; T[i] = 0; }
    f[0] = 1; f[1] = 8;      /* 1 + 8t */
    g[0] = 1; g[1] = -1;     /* 1 - t */

    a002521_pow(P1, f, 7, 10);

    a002521_pow(P2, f, 4, 10);
    a002521_pow(T, g, 3, 10);
    a002521_mul(P2, T, 10);
    for (i = 9; i >= 1; i--) P2[i] = P2[i - 1];
    P2[0] = 0;

    a002521_pow(P3, f, 1, 10);
    a002521_pow(T, g, 6, 10);
    a002521_mul(P3, T, 10);
    for (i = 9; i >= 2; i--) P3[i] = P3[i - 2];
    P3[0] = 0; P3[1] = 0;

    for (i = 0; i < 10; i++) W[i] = P1[i] - 56 * P2[i] + 280 * P3[i];
    return (seq_t)(uint64_t)W[(int)n];
}
