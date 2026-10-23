/*
 * A177757  Number of ways to place 4 nonattacking bishops on an n X n
 *          toroidal board.
 * https://oeis.org/A177757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/48*(n-2)^2*n^2* (2n^4 -16n^3 +50n^2 -84n +81 +(6n^2
 *            -36n +63)*(-1)^n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 25, u64 n <= 382, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t nm2 = sn - (seq_t)2;
    seq_t part1 = nm2 * nm2;          /* (n-2)^2 */
    seq_t part2 = sn * sn;            /* n^2 */

    seq_t n2 = part2;
    seq_t n3 = n2 * sn;
    seq_t n4 = n3 * sn;

    seq_t poly = (seq_t)2 * n4
               - (seq_t)16 * n3
               + (seq_t)50 * n2
               - (seq_t)84 * sn
               + (seq_t)81;

    seq_t add = (seq_t)6 * n2
              - (seq_t)36 * sn
              + (seq_t)63;

    if ((n & 1) == 0)          /* even n: (-1)^n = 1 */
        poly += add;
    else                       /* odd n: (-1)^n = -1 */
        poly -= add;

    /* Reduce division by 48 across the multiplicands to avoid overflow */
    seq_t a_mul = part1;
    seq_t b_mul = part2;
    seq_t c_mul = poly;
    seq_t remaining = (seq_t)48;

    /* Divide out factors of 2 (2^4 = 16) */
    for (int i = 0; i < 4; ++i) {
        if (remaining % (seq_t)2 == 0) {
            if (a_mul % (seq_t)2 == 0) {
                a_mul /= (seq_t)2;
                remaining /= (seq_t)2;
            } else if (b_mul % (seq_t)2 == 0) {
                b_mul /= (seq_t)2;
                remaining /= (seq_t)2;
            } else {
                c_mul /= (seq_t)2;
                remaining /= (seq_t)2;
            }
        }
    }

    /* Divide out factor of 3 */
    if (remaining % (seq_t)3 == 0) {
        if (a_mul % (seq_t)3 == 0) {
            a_mul /= (seq_t)3;
            remaining /= (seq_t)3;
        } else if (b_mul % (seq_t)3 == 0) {
            b_mul /= (seq_t)3;
            remaining /= (seq_t)3;
        } else {
            c_mul /= (seq_t)3;
            remaining /= (seq_t)3;
        }
    }

    /* The remaining should now be 1. In the rare case it is not,
       the following multiplication would still produce the correct
       result due to integer truncation (though the expression is
       mathematically guaranteed to be divisible). */
    (void)remaining;  /* silence unused warning */

    /* Final multiplication – no overflow if the result fits in seq_t */
    seq_t res = a_mul;
    res *= b_mul;
    res *= c_mul;
    return res;
}
