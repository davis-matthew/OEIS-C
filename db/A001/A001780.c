/*
 * A001780  Expansion of 1/((1+x)*(1-x)^9).
 * https://oeis.org/A001780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} binomial(n-2*j+7, 7), the
 *            coefficient of x^n in 1/((1-x^2)*(1-x)^8) = 1/((1+x)*(1-x)^9).
 *            Computed by taking the series of 1/(1-x^2) and applying 8
 *            running prefix sums
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 60, u64 n <= 1045,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t s[9];
    int j;
    idx_t k;
    for (j = 0; j < 9; j++) s[j] = 0;
    for (k = 0; k <= n; k++) {
        s[0] = (k % 2 == 0) ? 1 : 0;
        for (j = 1; j < 9; j++) s[j] += s[j - 1];
    }
    return s[8];
}
