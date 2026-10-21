/*
 * A238602  A sixth-order linear divisibility sequence related to the Pell
 *          numbers: a(n) := (1/60)*Pell(3*n)*Pell(4*n)/Pell(n).
 * https://oeis.org/A238602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            170*a(n-1)+5745*a(n-2)-40052*a(n-3)+5745*a(n-4)+170*a(n-5)-a(n-6);
 *            initial terms: a(1)=1, a(2)=238, a(3)=45507, a(4)=9063516,
 *            a(5)=1792708805, a(6)=355009117386
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[6] = {1, 238, 45507, 9063516, 1792708805ULL, 355009117386ULL};

    /* Handle the first six terms directly. */
    if (n <= 6)
        return init[n-1];

    /* Circular buffer holding the last six terms:
       buf[0] = a(i-6), buf[1] = a(i-5), … , buf[5] = a(i-1) */
    seq_t buf[6];
    for (idx_t i = 0; i < 6; ++i)
        buf[i] = init[i];

    /* Compute up to the desired n using the recurrence. */
    for (idx_t i = 7; i <= n; ++i) {
        seq_t sum_pos = buf[5] * (seq_t)170
                      + buf[4] * (seq_t)5745
                      + buf[2] * (seq_t)5745
                      + buf[1] * (seq_t)170;
        seq_t sub1   = buf[3] * (seq_t)40052;
        seq_t val    = sum_pos - (sub1 + buf[0]);

        /* Shift the buffer left and insert the new value. */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = val;
    }

    return buf[5];
}
