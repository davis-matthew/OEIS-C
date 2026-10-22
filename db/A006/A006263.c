/*
 * A006263  Erroneous version of A014221.
 * https://oeis.org/A006263
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2^^n-style tower as listed (erroneous A014221): a(0)=1,
 *            a(1)=2, a(2)=4, a(3) = 2^16; for n >= 3, a(n) = 2^e(n) where
 *            e(3) = 16 and e(n) = 2^e(n-1)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 3, i64 n <= 3, i128 n <= 3,
 *            big any n
 * Helpers:   pw2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pw2.h"

seq_t a(idx_t n) {
    idx_t e, i;
    if (n <= 2) return (seq_t)1 << n;
    e = 16;
    for (i = 3; i < n; i++)
        e = (idx_t)1 << e; /* only reached for n >= 4, where values are astronomically large */
    return pw2(e);
}
