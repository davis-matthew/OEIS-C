/*
 * A162495  Number of reduced words of length n in the icosahedral reflection
 *          group [3,5] of order 120.
 * https://oeis.org/A162495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = coefficient of x^n in (1-x^2)(1-x^6)(1-x^10)/(1-x)^3
 *            which equals sum_{k in {0,2,6,8,10,12,16,18}} c_k *
 *            binom(n-k+2,2), with c = {+1,-1,-1,+1,-1,+1,+1,-1}
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* coefficients and corresponding shift values */
    static const int ks[8]  = {0, 2, 6, 8, 10, 12, 16, 18};
    static const int cof[8] = {1, -1, -1, 1, -1, 1, 1, -1};

    int64_t sum = 0;                      /* accumulator (signed) */
    for (int i = 0; i < 8; ++i) {
        if (n >= (idx_t)ks[i]) {
            idx_t m = n - (idx_t)ks[i] + 2;            /* m = n-k+2 */
            int64_t term = (int64_t)m * (int64_t)(m - 1) / 2; /* binom(m,2) */
            sum += (cof[i] > 0) ? term : -term;
        }
    }
    return (seq_t)sum;                /* final value fits in seq_t */
}
