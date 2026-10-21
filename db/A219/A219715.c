/*
 * A219715  Number of 2 X n arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random, but sorted with lexicographically nondecreasing rows and
 *          nonincreasing columns, 0..3 2 X n array.
 * https://oeis.org/A219715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6); Initial values: a(1)=10, a(2)=13, a(3)=45, a(4)=125,
 *            a(5)=283, a(6)=561, a(7)=1030, a(8)=1776, a(9)=2916, a(10)=4610.
 *            This recurrence holds for all n > 10
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 138, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 10 terms of the sequence */
    static const seq_t init[10] = {
        10, 13, 45, 125, 283, 561,
        1030, 1776, 2916, 4610
    };

    if (n <= 10)
        return init[n - 1];

    /* circular buffer holding the last 6 computed values:
       v[0] = a(i-6), v[1] = a(i-5), ..., v[5] = a(i-1) for current i */
    seq_t v[6];
    v[0] = init[4]; /* a(5) */
    v[1] = init[5]; /* a(6) */
    v[2] = init[6]; /* a(7) */
    v[3] = init[7]; /* a(8) */
    v[4] = init[8]; /* a(9) */
    v[5] = init[9]; /* a(10) */

    for (idx_t i = 11; i <= n; ++i) {
        seq_t val = 6 * v[5]          /* a(i-1) */
                  - 15 * v[4]          /* a(i-2) */
                  + 20 * v[3]          /* a(i-3) */
                  - 15 * v[2]          /* a(i-4) */
                  + 6  * v[1]          /* a(i-5) */
                  -   v[0];            /* a(i-6) */

        /* shift buffer forward */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = val;
    }

    return v[5];
}
