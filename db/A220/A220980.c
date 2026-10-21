/*
 * A220980  a(n) = 5^(4n+2) + 5^(3n+2) + 3 * 5^(2n+1) + 5^(n+1) + 1: the right
 *          Aurifeuillian factor of 5^(10n+5) - 1.
 * https://oeis.org/A220980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(4*n+2) + 5^(3*n+2) + 3*5^(2*n+1) + 5^(n+1) + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/power5.h"

seq_t a(idx_t n) {
    /* compute each power of 5 */
    seq_t p1 = power5(4 * n + 2);   /* 5^(4n+2) */
    seq_t p2 = power5(3 * n + 2);   /* 5^(3n+2) */
    seq_t p3 = power5(2 * n + 1);   /* 5^(2n+1) */
    seq_t p4 = power5(n + 1);       /* 5^(n+1) */

    p3 *= 3;                        /* 3 * 5^(2n+1) */

    /* final sum */
    seq_t res = p1 + p2 + p3 + p4 + 1;
    return res;
}
