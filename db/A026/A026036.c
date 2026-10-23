/*
 * A026036  (d(n)-r(n))/5, where d = A006527 and r is the periodic sequence
 *          with fundamental period (4,1,4,0,1).
 * https://oeis.org/A026036
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            [(2-2x+x^2)(1+x^2)x^3]/[(1-x)^3(1-x^5)]
 * Range:     u8 n <= 15, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    static const int coeff[5] = {2, -2, 3, -2, 1};
    static const int shift[5] = {3, 4, 5, 6, 7};
    int64_t sum = 0;
    for (int i = 0; i < 5; ++i) {
        int p = shift[i];
        int c = coeff[i];
        if (n < (idx_t)p) continue;
        idx_t maxj = (n - (idx_t)p) / 5;
        for (idx_t j = 0; j <= maxj; ++j) {
            idx_t k = n - (idx_t)p - 5 * j;
            int64_t term = ((int64_t)(k + 2) * (k + 1)) / 2;
            if (c > 0)
                sum += (int64_t)c * term;
            else
                sum -= (int64_t)(-c) * term;
        }
    }
    return (seq_t)sum;
}
