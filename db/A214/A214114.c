/*
 * A214114  Number of 0..3 colorings of a 3X(n+1) array circular in the n+1
 *          direction with new values 0..3 introduced in row major order.
 * https://oeis.org/A214114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1)+123*a(n-2)-509*a(n-3)-1615*a(n-4)+7137*a(n-5) *
 *            -19*a(n-6)-20571*a(n-7)+13176*a(n-8)+13932*a(n-9)-11664*a(n-10)
 *            * with initial values a(1)=25, a(2)=121, ..., a(10)=928191589611
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases for n = 1 .. 10 */
    static const seq_t base[10] = {
        (seq_t)25,
        (seq_t)121,
        (seq_t)3502,
        (seq_t)44900,
        (seq_t)825105,
        (seq_t)12777541,
        (seq_t)214404272,
        (seq_t)3462096250,
        (seq_t)56936675435,
        (seq_t)928191589611
    };

    if (n <= 10) {
        return base[n-1];
    }

    /* Work with the last 10 terms */
    seq_t a1 = base[0], a2 = base[1], a3 = base[2], a4 = base[3], a5 = base[4],
          a6 = base[5], a7 = base[6], a8 = base[7], a9 = base[8], a10 = base[9];
    seq_t next = 0;

    for (idx_t i = 11; i <= n; ++i) {
        next = (seq_t)11 * a10
               + (seq_t)123 * a9
               - (seq_t)509 * a8
               - (seq_t)1615 * a7
               + (seq_t)7137 * a6
               - (seq_t)19 * a5
               - (seq_t)20571 * a4
               + (seq_t)13176 * a3
               + (seq_t)13932 * a2
               - (seq_t)11664 * a1;

        /* shift the window of last 10 terms */
        a1 = a2; a2 = a3; a3 = a4; a4 = a5; a5 = a6;
        a6 = a7; a7 = a8; a8 = a9; a9 = a10; a10 = next;
    }

    return next;
}
