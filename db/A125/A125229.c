/*
 * A125229  a(n) = j such that i^j is maximized subject to i+j = n (i >= 0, j
 *          >= 0).
 * https://oeis.org/A125229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 317, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Search: maximize i^(n-i); return j = n-i.
   For ties choose j = 0 (i = n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t i = 1;
    const double eps = 1e-12;

    while (i < n) {
        double left  = (double)(n - i) * log((double)i);
        double right = (double)(n - i - 1) * log((double)(i + 1));
        if (left <= right + eps) {
            ++i;               /* move to larger base if it does not decrease */
        } else {
            break;             /* current i gives the maximum */
        }
    }

    return (seq_t)(n - i);      /* j = n - i */
}
