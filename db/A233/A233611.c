/*
 * A233611  Number of (n+2) X (1+2) 0..2 arrays with no increasing sequence of
 *          length 3 horizontally or diagonally downwards.
 * https://oeis.org/A233611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26*a(n-1) - 16*a(n-2) - 88*a(n-3) + 48*a(n-4) with
 *            initial values: a(1) = 17064 a(2) = 430608 a(3) = 10864544 a(4)
 *            = 274119232
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 1) return (seq_t)17064;
    if (n == 2) return (seq_t)430608;
    if (n == 3) return (seq_t)10864544;
    if (n == 4) return (seq_t)274119232;

    /* Variables for the four previous values */
    seq_t a1 = (seq_t)17064;
    seq_t a2 = (seq_t)430608;
    seq_t a3 = (seq_t)10864544;
    seq_t a4 = (seq_t)274119232;
    seq_t next = 0;

    /* Iteratively compute up to a(n) */
    for (idx_t i = 5; i <= n; ++i) {
        next = 26 * a4;
        next -= 16 * a3;
        next -= 88 * a2;
        next += 48 * a1;

        /* Shift values for next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
