/*
 * A166213  Number of 5 X 5 X 5 triangular nonnegative integer arrays,
 *          symmetric under 120 degree rotation, with all sums of an element
 *          and its neighbors <= n.
 * https://oeis.org/A166213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - 3*a(n-4) + 6*a(n-6) -
 *            3*a(n-8) - 2*a(n-9) + a(n-10) + 2*a(n-11) - a(n-12) Initial
 *            values: a(0)=1, a(1)=4, a(2)=12, a(3)=30, a(4)=64, a(5)=122,
 *            a(6)=218, a(7)=364, a(8)=579, a(9)=886, a(10)=1310, a(11)=1880
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* First 12 terms of the sequence */
    static const seq_t init[12] = {
        1,   4,   12,   30,   64,   122,   218,
        364, 579, 886,  1310, 1880
    };
    if (n < 12) {
        return init[n];
    }

    /* Circular buffer holding the last 13 computed values */
    seq_t buf[13];
    for (size_t i = 0; i < 12; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 12; i <= n; ++i) {
        size_t i1  = (size_t)((i - 1) % 13);
        size_t i2  = (size_t)((i - 2) % 13);
        size_t i3  = (size_t)((i - 3) % 13);
        size_t i4  = (size_t)((i - 4) % 13);
        size_t i6  = (size_t)((i - 6) % 13);
        size_t i8  = (size_t)((i - 8) % 13);
        size_t i9  = (size_t)((i - 9) % 13);
        size_t i10 = (size_t)((i -10) % 13);
        size_t i11 = (size_t)((i -11) % 13);
        size_t i12 = (size_t)((i -12) % 13);

        seq_t sum = 0;
        sum = sum + 2 * buf[i1];
        sum = sum + buf[i2];
        sum = sum - 2 * buf[i3];
        sum = sum - 3 * buf[i4];
        sum = sum + 6 * buf[i6];
        sum = sum - 3 * buf[i8];
        sum = sum - 2 * buf[i9];
        sum = sum + buf[i10];
        sum = sum + 2 * buf[i11];
        sum = sum - buf[i12];

        buf[i % 13] = sum;
    }

    return buf[n % 13];
}
