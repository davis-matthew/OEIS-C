/*
 * A102565  a(n) = A102563(n) - A102564(n).
 * https://oeis.org/A102565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=0; for even n>0, a(n)=1 if popcount(n/2) odd else -1; for
 *            odd n, a(n)=-2 if popcount((n-1)/2) odd else 0
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   popcnt64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/popcnt64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n & 1) {                      /* n is odd */
        idx_t k = (n - 1) >> 1;
        unsigned ones = popcnt64(k);
        return (seq_t)(-2) * (seq_t)(ones & 1u);
    } else {                           /* n is even */
        idx_t k = n >> 1;
        if (k == 0) return 0;
        unsigned ones = popcnt64(k);
        return (seq_t)((ones & 1u) ? 1 : -1);
    }
}
