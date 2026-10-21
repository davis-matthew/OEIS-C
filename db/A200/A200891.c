/*
 * A200891  Number of 0..n arrays x(0..7) of 8 elements without any interior
 *          element greater than both neighbors.
 * https://oeis.org/A200891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) * +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) * for n >
 *            9; initial values: * a(1)=114, a(2)=1691, a(3)=11472,
 *            a(4)=50995, a(5)=173606, * a(6)=491533, a(7)=1215616,
 *            a(8)=2710413, a(9)=5567530
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 25, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial values for n = 1..9 */
    static const seq_t init[9] = {
        114ull,
        1691ull,
        11472ull,
        50995ull,
        173606ull,
        491533ull,
        1215616ull,
        2710413ull,
        5567530ull
    };

    if (n <= 9) {
        return init[n - 1];
    }

    /* buffer holds a(k-9) .. a(k-1), where k is the next n to compute */
    seq_t buf[9];
    for (idx_t i = 0; i < 9; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 10; k <= n; ++k) {
        seq_t temp = 0;
        temp += buf[8] * 9;
        temp -= buf[7] * 36;
        temp += buf[6] * 84;
        temp -= buf[5] * 126;
        temp += buf[4] * 126;
        temp -= buf[3] * 84;
        temp += buf[2] * 36;
        temp -= buf[1] * 9;
        temp += buf[0];

        /* shift buffer left by one */
        for (idx_t i = 0; i < 8; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[8] = temp;
    }

    return buf[8];
}
