/*
 * A157427  Lower left-hand y-coordinates of invisible 1x1 squares with 0 < y
 *          < x.
 * https://oeis.org/A157427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the y‑coordinate of the nth invisible 1×1 square * with 0
 *            < y < x, enumerated by increasing x then y
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* Undefined for n = 0; return 0 as a fallback. */
        return (seq_t)0;
    }

    idx_t count = (idx_t)0;

    for (idx_t x = (idx_t)2; ; ++x) {
        seq_t xs  = (seq_t)x;
        seq_t xs1 = xs + (seq_t)1;

        for (idx_t y = (idx_t)1; y < x; ++y) {
            seq_t ys  = (seq_t)y;
            seq_t ys1 = ys + (seq_t)1;

            if (gcd(xs,  ys)  != (seq_t)1 &&
                gcd(xs,  ys1) != (seq_t)1 &&
                gcd(xs1, ys)  != (seq_t)1 &&
                gcd(xs1, ys1) != (seq_t)1)
            {
                ++count;
                if (count == n) {
                    return ys;
                }
            }
        }
    }

    /* Unreachable, added to silence compiler warnings. */
    return (seq_t)0;
}
