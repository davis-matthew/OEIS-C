/*
 * A018764  Divisors of 994.
 * https://oeis.org/A018764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = nth divisor of 994 (sorted ascending)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    /* There are exactly 8 divisors: 1, 2, 7, 14, 71, 142, 497, 994 */
    seq_t divs[8];
    int i = 0;
    int a1, b1, c1;
    for (a1 = 0; a1 <= 1; ++a1) {
        for (b1 = 0; b1 <= 1; ++b1) {
            for (c1 = 0; c1 <= 1; ++c1) {
                seq_t d = (seq_t)1;
                if (a1) d *= (seq_t)2;
                if (b1) d *= (seq_t)7;
                if (c1) d *= (seq_t)71;
                divs[i++] = d;
            }
        }
    }
    /* sort the 8 divisors */
    for (int p = 0; p < 7; ++p) {
        for (int q = p + 1; q < 8; ++q) {
            if (divs[q] < divs[p]) {
                seq_t t = divs[p];
                divs[p] = divs[q];
                divs[q] = t;
            }
        }
    }
    if (n - 1 >= 8) return (seq_t)0;
    return divs[n - 1];
}
