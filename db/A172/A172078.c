/*
 * A172078  a(n) = n*(16*n^2 + 3*n - 13)/6.
 * https://oeis.org/A172078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(16*n-13)/6
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 849, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    if (nn == 0) {
        return 0;
    }
    /* first divide n*(n+1) by 2 to keep numbers smaller; the product is always even */
    seq_t t = nn * (nn + 1) / 2;
    /* now multiply by (16*n-13) and finally divide by 3; the overall quotient is integral */
    seq_t term = t * (16 * nn - 13) / 3;
    return term;
}
