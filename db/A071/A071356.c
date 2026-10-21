/*
 * A071356  Expansion of (1 - 2*x - sqrt(1 - 4*x - 4*x^2))/(4*x^2).
 * https://oeis.org/A071356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binom(n, 2k) * Catalan(k) * 2^{n-k}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t maxK = n / 2;
    seq_t sum = 0;
    seq_t binom = 1;      /* binom(n, 0) */
    seq_t cat   = 1;      /* Catalan(0) */

    for (idx_t k = 0; k <= maxK; ++k) {
        /* term = binom * cat * 2^{n-k} */
        seq_t term = binom;
        term = term * cat;
        term = term * pow2(n - k);
        sum += term;

        if (k == maxK) break;          /* last iteration: no need to update */

        /* update binom to binom(n, 2(k+1)) */
        idx_t m = 2 * k;                /* current m = 2k */
        seq_t factor1 = (seq_t)(n - m);
        seq_t factor2 = (seq_t)(n - m - 1);
        seq_t denom1  = (seq_t)(m + 1);
        seq_t denom2  = (seq_t)(m + 2);
        binom = binom * factor1 * factor2 / (denom1 * denom2);

        /* update Catalan to Catalan(k+1) */
        idx_t kp1 = k + 1;
        seq_t cat_num = (seq_t)(2 * (2 * kp1 - 1)); /* 2*(2k-1) */
        seq_t cat_den = (seq_t)(kp1 + 1);           /* k+2 */
        cat = cat * cat_num / cat_den;
    }

    return sum;
}
