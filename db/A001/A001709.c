/*
 * A001709  Generalized Stirling numbers, [n+7,7]_2.
 * https://oeis.org/A001709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(k+5,5)*2^k*|Stirling1(n+5,k+5)| = [t^5]
 *            Product_{i=2..n+6} (t+i), computed by multiplying out the
 *            product and keeping only the coefficients of t^0..t^5
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t c[6];
    idx_t i;
    int k;
    for (k = 0; k < 6; k++) c[k] = 0;
    c[0] = 1;
    for (i = 2; i <= n + 6; i++) {
        for (k = 5; k >= 1; k--) {
            c[k] = c[k] * (seq_t)i + c[k - 1];
        }
        c[0] = c[0] * (seq_t)i;
    }
    return c[5];
}
