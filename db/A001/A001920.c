/*
 * A001920  Expansion of 1/(1+759*x^2+2576*x^3+759*x^4+x^6).
 * https://oeis.org/A001920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -759*a(n-2) - 2576*a(n-3) - 759*a(n-4) - a(n-6), a(0) =
 *            1, a(n) = 0 for n < 0 (so a(1) = 0, a(2) = -759, a(3) = -2576,
 *            a(4) = 575322, a(5) = 3910368)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 13, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* p1..p6 hold a(i-1)..a(i-6) */
    seq_t p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0;
    seq_t cur = 0;
    idx_t i;
    for (i = 0; i <= n; i++) {
        cur = 0;
        if (i == 0) cur = 1;
        cur -= p2 * 759;
        cur -= p3 * 2576;
        cur -= p4 * 759;
        cur -= p6;
        p6 = p5;
        p5 = p4;
        p4 = p3;
        p3 = p2;
        p2 = p1;
        p1 = cur;
    }
    return cur;
}
