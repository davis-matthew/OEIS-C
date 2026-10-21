/*
 * A207644  a(n) = 1 + (n-1) + (n-2)*[(n-3)/2] + (n-3)*[(n-4)/2]*[(n-5)/3] +
 *          (n-4)*[(n-5)/2]*[(n-6)/3]*[(n-7)/4] +... where [x] = floor(x),
 *          with summation extending over the initial [n/2+1] products only.
 * https://oeis.org/A207644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 1 + Sum_{k=1..floor(n/2)} Product_{j=1..k}
 *            floor((n-k-j+1)/j)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 50, u64 n <= 99, u128 n <= 198,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 1;                 /* the leading 1 */

    /* number of terms in the sum */
    idx_t maxk = n / 2;
    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t prod = 1;
        for (idx_t j = 1; j <= k; ++j) {
            seq_t term = (seq_t)(n - k - j + 1) / j; /* floor division */
            if (!term) {          /* once a factor is 0, whole product is 0 */
                prod = 0;
                break;
            }
            prod *= term;
        }
        sum += prod;
    }
    return sum;
}
