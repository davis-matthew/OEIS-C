/*
 * A058410  Number of 4 X n nonnegative integer matrices with all column sums
 *          4, up to row and column permutation.
 * https://oeis.org/A058410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1/24 * ( (1-x)^-35 + 6*(1-x)^-9*(1-x^2)^-13 +
 *            3*(1-x)^-3*(1-x^2)^-16 + 8*(1-x)^-2*(1-x^3)^-11 +
 *            6*(1-x)^-1*(1-x^2)^-1*(1-x^4)^-8 ) The coefficient of x^n in
 *            each term is evaluated directly
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_fast.h"

seq_t a(idx_t n)
{
    /* Term 1: 1/(1-x)^35 */
    seq_t term1 = binom_fast(n + 34, 34);

    /* Term 2: 6/(1-x)^9/(1-x^2)^13 */
    seq_t term2 = 0;
    idx_t lim2 = n / 2;
    for (idx_t m = 0; m <= lim2; ++m) {
        seq_t c1 = binom_fast(n - 2 * m + 8, 8);
        seq_t c2 = binom_fast(m + 12, 12);
        term2 += c1 * c2;
    }

    /* Term 3: 3/(1-x)^3/(1-x^2)^16 */
    seq_t term3 = 0;
    for (idx_t m = 0; m <= lim2; ++m) {
        seq_t c1 = binom_fast(n - 2 * m + 2, 2);
        seq_t c2 = binom_fast(m + 15, 15);
        term3 += c1 * c2;
    }

    /* Term 4: 8/(1-x)^2/(1-x^3)^11 */
    seq_t term4 = 0;
    idx_t lim3 = n / 3;
    for (idx_t m = 0; m <= lim3; ++m) {
        seq_t c1 = n - 3 * m + 1;               /* binom(n-3m+1,1) */
        seq_t c2 = binom_fast(m + 10, 10);
        term4 += c1 * c2;
    }

    /* Term 5: 6/(1-x)/(1-x^2)/(1-x^4)^8 */
    seq_t term5 = 0;
    idx_t lim4 = n / 4;
    for (idx_t c = 0; c <= lim4; ++c) {
        seq_t coef = binom_fast(c + 7, 7);           /* from (1-x^4)^-8 */
        idx_t remaining = n - 4 * c;
        idx_t m2_count = (remaining / 2) + 1;        /* solutions to a+2b=remaining */
        term5 += coef * m2_count;
    }

    seq_t total = term1
                + term2 * 6
                + term3 * 3
                + term4 * 8
                + term5 * 6;
    return total / 24;
}
