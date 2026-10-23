/*
 * A179894  Given the series (1, 2, 1, 2, 1, 2, ...), let (1 + 2x + x^2 + 2x^3
 *          + ...) * (1 + 2x^2 + x^3 + 2x^4 + ...) = (1 + 2x + 3x^2 + 7x^3 +
 *          ...)
 * https://oeis.org/A179894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = ((9 + (-1)^n)*n - 12)/4
 *            Equivalently: a(n) = 2*n - 3 for odd n > 1, and a(n) = 5*n/2 - 3
 *            for even n
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1)
        return 1;
    if (n & 1)                 /* odd n > 1 */
        return ((seq_t)2 * n - 3);
    else                       /* even n */
        return ((seq_t)5 * n / 2 - 3);
}
