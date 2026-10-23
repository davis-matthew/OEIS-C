/*
 * A175243  Array read by antidiagonals: total number of spanning trees R_n(m)
 *          of the complete prism K_m X C_n.
 * https://oeis.org/A175243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = R_{row}(col) where (row,col) is the antidiagonal *
 *            coordinate of n; R_n(m) = n * 2^(m-1) * (T_n(1+m/2)-1)^(m-1) / m
 *            * with T_n(x) the Chebyshev polynomial of the first kind
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t, idx_t) */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t, seq_t), optional */

seq_t a(idx_t n) {
    /* Convert index n into (row, col) by antidiagonal order */
    idx_t k = n;
    idx_t s = 1;
    idx_t cum = 1;                /* sum_{t=1}^{s} t */
    while (cum < k) {
        s++;
        cum += s;                 /* cum = s(s+1)/2 after loop */
    }
    idx_t prev = cum - s;          /* sum_{t=1}^{s-1} t = (s-1)s/2 */
    idx_t offset = k - prev;       /* 1 .. s */
    idx_t row = s - offset + 1;
    idx_t col = offset;

    /* Compute R_row(col) */
    idx_t m = col;
    idx_t nn = row;

    /* Even m */
    if ((m & 1ULL) == 0) {
        seq_t x = (seq_t)(1 + m / 2);   /* integer */
        seq_t T0 = 1;
        seq_t T1 = x;
        if (nn >= 2) {
            for (idx_t i = 2; i <= nn; i++) {
                seq_t Ti = (seq_t)2 * x * T1 - T0;
                T0 = T1;
                T1 = Ti;
            }
        }
        seq_t y = T1 - 1;              /* integer */
        seq_t pow2 = seq_pow((seq_t)2, m - 1);
        seq_t bigpow = seq_pow(y, m - 1);
        seq_t tmp = bigpow * pow2;
        tmp = tmp * (seq_t)nn;
        tmp = tmp / (seq_t)m;
        return tmp;
    }

    /* Odd m */
    /* Compute S_n where S_n = 2^n * T_n(a/2),  a = m + 2 (= 2k+3) */
    seq_t a_val = (seq_t)(m + 2);          /* integer a */
    seq_t S0 = 1;
    seq_t S1 = a_val;
    if (nn >= 2) {
        for (idx_t i = 2; i <= nn; i++) {
            seq_t Si = (seq_t)2 * a_val * S1 - (seq_t)4 * S0;
            S0 = S1;
            S1 = Si;
        }
    }
    seq_t two_pow_n = seq_pow((seq_t)2, nn);
    seq_t numerator = S1 - two_pow_n;
    seq_t denom = seq_pow((seq_t)2, nn - 1);    /* 2^(n-1) */
    seq_t y2 = numerator / denom;              /* 2*(T_n(x)-1), integer */
    seq_t bigpow = seq_pow(y2, m - 1);
    seq_t tmp = bigpow * (seq_t)nn;
    tmp = tmp / (seq_t)m;
    return tmp;
}
