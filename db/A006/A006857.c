/*
 * A006857  a(n) = binomial(n+5,5) * binomial(n+5,4)/(n+5).
 * https://oeis.org/A006857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+4,4)*C(n+5,4)/5
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 40, u64 n <= 689, u128 n <= 1000,
 *            big any n
 * Helpers:   a006857_binom4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a006857_binom4.h"

seq_t a(idx_t n) {
    seq_t x = a006857_binom4(n + 4);
    seq_t y = a006857_binom4(n + 5);
    if (x % 5 == 0)
        x /= 5;
    else
        y /= 5;
    return x * y;
}
