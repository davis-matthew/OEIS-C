/*
 * A056323  Number of reversible string structures with n beads using a
 *          maximum of four different colors.
 * https://oeis.org/A056323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16 + (-2)^n + 15*2^n + 4^n) / 48 (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t pow2n = pow2(n);   /* 2^n */
    seq_t pow4n = pow4(n);   /* 4^n */

    seq_t numerator;
    if (n & 1) { /* n is odd: (-2)^n = -2^n  → 16 + 14*2^n + 4^n */
        numerator = pow2n * (seq_t)14 + pow4n + (seq_t)16;
    } else {     /* n is even: (-2)^n =  2^n  → 16 + 16*2^n + 4^n */
        numerator = pow2n * (seq_t)16 + pow4n + (seq_t)16;
    }

    return numerator / (seq_t)48;
}
