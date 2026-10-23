/*
 * A186190  First digit of tribonacci sequence A000213.
 * https://oeis.org/A186190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = first digit of tribonacci number T_n with T_0=T_1=T_2=1,
 *            T_k = T_{k-1}+T_{k-2}+T_{k-3} (offset 0)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the tribonacci numbers are 1,1,1 → first digit 1. */
    if (n < 3)
        return (seq_t)1;

    /* L0, L1, L2 store log10(T_{i-3}), log10(T_{i-2}), log10(T_{i-1}) respectively. */
    double L0 = 0.0, L1 = 0.0, L2 = 0.0;
    double Ln = 0.0;                      /* will hold log10(T_i) */

    for (idx_t i = 3; i <= n; ++i) {
        double d1 = pow(10.0, L2 - L0);   /* 10^(L_{i-1} - L_{i-3}) */
        double d2 = pow(10.0, L1 - L0);   /* 10^(L_{i-2} - L_{i-3}) */
        double sum = d1 + d2 + 1.0;        /* inside log10 */
        Ln = L0 + log10(sum);             /* log10(T_i) */

        /* shift registers for next iteration */
        L0 = L1;
        L1 = L2;
        L2 = Ln;
    }

    /* compute first digit from fractional part of log10(T_n) */
    double frac = Ln - floor(Ln);
    double first = pow(10.0, frac);
    int digit = (int)floor(first + 1e-12);   /* tiny epsilon to avoid 9.999... */
    return (seq_t)digit;
}
