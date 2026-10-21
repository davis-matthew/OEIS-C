/*
 * A219010  Numerators in a product expansion for sqrt(5).
 * https://oeis.org/A219010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3; a(n+1) = a(n)^5 - 5*a(n)^3 + 5*a(n)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)3;
    for (idx_t i = 0; i < n; ++i) {
        seq_t x2 = res * res;                 /* a^2          */
        seq_t x3 = x2 * res;                  /* a^3          */
        seq_t x5 = x3 * x2;                  /* a^5          */
        res = x5 - (seq_t)5 * x3 + (seq_t)5 * res;
    }
    return res;
}
