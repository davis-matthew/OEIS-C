/*
 * A195855  a(n) = T(9,n), array T given by A048505.
 * https://oeis.org/A195855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 37*n + 324) * 2^(n-2) - 80
 * Range:     u8 none, u16 n <= 8, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Basic quadratic factor */
    seq_t base = (seq_t)n * (seq_t)n + (seq_t)37 * (seq_t)n + (seq_t)324;

    seq_t mult;
    if (n == 0) {
        /* 2^(n-2) = 1/4 */
        mult = base / 4;
    } else if (n == 1) {
        /* 2^(n-2) = 1/2 */
        mult = base / 2;
    } else {
        /* 2^(n-2) for n >= 2 is an integer power of two */
        mult = base * pow2(n - 2);
    }

    return mult - (seq_t)80;
}
