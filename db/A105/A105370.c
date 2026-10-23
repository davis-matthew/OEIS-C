/*
 * A105370  Expansion of ((1+x)^4-(1+x)x^3)/((1+x)^5-x^5).
 * https://oeis.org/A105370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -5*a(n-1)-10*a(n-2)-10*a(n-3)-5*a(n-4) with initial
 *            values a(0) = 1, a(1) = -1, a(2) = 1, a(3) = -2
 * Range:     i8 n <= 9, i16 n <= 19, i32 n <= 34, i64 n <= 38, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the initial four terms directly. */
    if (n < 4) {
        static const seq_t init[4] = { 1, -1, 1, -2 };
        return init[n];
    }

    /* Iteratively compute terms from 4 up to n. */
    seq_t a0 = 1;   /* a(n-4)  */
    seq_t a1 = -1;  /* a(n-3)  */
    seq_t a2 = 1;   /* a(n-2)  */
    seq_t a3 = -2;  /* a(n-1)  */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(n) = -5*a(n-1) -10*a(n-2) -10*a(n-3) -5*a(n-4) */
        next = -(seq_t)5 * a3
             - (seq_t)10 * a2
             - (seq_t)10 * a1
             - (seq_t)5 * a0;

        /* Shift the sliding window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;   /* a(n) is now in a3 */
}
