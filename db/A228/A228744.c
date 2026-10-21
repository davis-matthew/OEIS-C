/*
 * A228744  Number of arrays of the median of three adjacent elements of some
 *          length 9 0..n array.
 * https://oeis.org/A228744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) – 28*a(n-2) + 56*a(n-3) – 70*a(n-4) + 56*a(n-5)
 *            – 28*a(n-6) + 8*a(n-7) – a(n-8) Initial values (offset 1):
 *            a(1)=88, a(2)=1071, a(3)=6130, a(4)=23627, a(5)=71278,
 *            a(6)=181885, a(7)=410828, a(8)=845517
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Initial values */
    static const seq_t init[8] = {
        (seq_t)88,
        (seq_t)1071,
        (seq_t)6130,
        (seq_t)23627,
        (seq_t)71278,
        (seq_t)181885,
        (seq_t)410828,
        (seq_t)845517
    };
    if (n <= 8) {
        return init[n - 1];
    }

    /* Circular buffer holding the last 8 terms:
       last[0] = a(k-1), last[1] = a(k-2), ..., last[7] = a(k-8)   */
    seq_t last[8];
    /* Initialize for k = 9: last[0] = a8, last[1] = a7, ..., last[7] = a1 */
    for (idx_t i = 0; i < 8; ++i) {
        last[i] = init[7 - i];
    }

    for (idx_t k = 9; k <= n; ++k) {
        seq_t positive = last[0] * (seq_t)8 + last[2] * (seq_t)56
                       + last[4] * (seq_t)56 + last[6] * (seq_t)8;
        seq_t negative = last[1] * (seq_t)28 + last[3] * (seq_t)70
                       + last[5] * (seq_t)28 + last[7];
        seq_t next = positive - negative;

        /* Shift the buffer for the next iteration */
        for (idx_t i = 7; i > 0; --i) {
            last[i] = last[i - 1];
        }
        last[0] = next;
    }

    return last[0];
}
