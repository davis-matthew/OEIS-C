/*
 * A003467  Number of minimal covers of an n-set that cover exactly 3 points
 *          uniquely.
 * https://oeis.org/A003467
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3) = 5; for n > 3, a(n) = n*(n-1)*(n-2)*(4^n+192)/384 =
 *            binomial(n,3)*(4^(n-3)+3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   a003467_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a003467_pow.h"

seq_t a(idx_t n) {
    if (n <= 3) {
        return (seq_t)5;
    }
    idx_t x = n, y = n - 1, z = n - 2;
    /* divide out 2 and 3 exactly from the factors before multiplying */
    if (x % 2 == 0) x /= 2; else y /= 2;
    if (x % 3 == 0) x /= 3;
    else if (y % 3 == 0) y /= 3;
    else z /= 3;
    seq_t c3 = (seq_t)x * (seq_t)y * (seq_t)z;
    seq_t p = a003467_pow((seq_t)4, n - 3);
    return c3 * (p + (seq_t)3);
}
