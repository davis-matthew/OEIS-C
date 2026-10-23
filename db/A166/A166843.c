/*
 * A166843  Number of nX3 1..4 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          decreasing order.
 * https://oeis.org/A166843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (n-1) * P(n) / 181440 where P(n) = 29*n^8 + 569*n^7 +
 *            8015*n^6 + 51863*n^5 + 197372*n^4 + 276752*n^3 + 10296*n^2 -
 *            22752*n - 181440 This is the empirical polynomial obtained from
 *            OEIS A166843
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* values for n<1 are undefined */
    /* Coefficients of P(n) from highest degree to constant term */
    static const int coeff[9] = {29, 569, 8015, 51863, 197372,
                                 276752, 10296, -22752, -181440};

    seq_t num = (seq_t)coeff[0];
    for (int i = 1; i < 9; ++i) {
        num = num * (seq_t)n;
        if (coeff[i] >= 0)
            num = num + (seq_t)coeff[i];
        else
            num = num - (seq_t)(-coeff[i]);
    }
    /* multiply by (n-1) and divide by 181440 (exact division) */
    num = num * (seq_t)(n - 1);
    return num / (seq_t)181440;
}
