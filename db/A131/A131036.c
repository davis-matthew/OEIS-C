/*
 * A131036  First differences of A131711.
 * https://oeis.org/A131036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -a(n-1) -a(n-4) -a(n-5) -a(n-8) -a(n-9) for n > 8, a(n) =
 *            a(n-12) (period 12) Initial values: a(0)=1, a(1)=1, a(2)=3,
 *            a(3)=-3, a(4)=7, a(5)=-9, a(6)=9, a(7)=-1, a(8)=-3, a(9)=3,
 *            a(10)=-7, a(11)=-1
 * Range:     i8 n <= 47, i16 n <= 47, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t period[12];
    static int ready = 0;

    if (!ready) {
        period[0] = 1;
        period[1] = 1;
        period[2] = 3;
        period[3] = -3;
        period[4] = 7;
        period[5] = -9;
        period[6] = 9;
        period[7] = -1;
        period[8] = -3;

        /* compute remaining terms of the period using the recurrence */
        for (idx_t i = 9; i <= 11; ++i) {
            period[i] = - period[i-1] - period[i-4]
                      - period[i-5] - period[i-8] - period[i-9];
        }
        ready = 1;
    }

    return period[n % 12];
}
