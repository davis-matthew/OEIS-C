/*
 * A145751  Duplicate of A063759.
 * https://oeis.org/A145751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(0) = 1; for n ≥ 1, if n is even → a(n) = 2^(n/2 + 1); if n is
 *            odd → a(n) = 3 * 2^((n-1)/2)
 * Range:     i8 n <= 11, i16 n <= 27, i32 n <= 44, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    if ((n & 1) == 0) {        /* n is even */
        idx_t k = n / 2;       /* k ≥ 1 for n ≥ 2 */
        return pow2(k + 1);
    } else {                   /* n is odd */
        idx_t k = (n - 1) / 2;
        return (seq_t)3 * pow2(k);
    }
}
