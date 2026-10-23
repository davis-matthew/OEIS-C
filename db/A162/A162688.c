/*
 * A162688  Strictly positive numbers k such that 12*k/(12+k) are integers.
 * https://oeis.org/A162688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   k is a positive integer such that 12*k/(12+k) is an integer.
 *            Equivalently, k = 12*j/(12-j) with integer j. Enumerate j =
 *            1..11; compute k if numerator is divisible by denominator. The
 *            resulting k values are in strictly increasing order. The
 *            sequence is finite (only 7 terms)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build the list of valid k's once, on the first call. */
    static bool built = false;
    static seq_t seq[7]; /* holds 4,6,12,24,36,60,132 */
    static idx_t cnt = 0;

    if (!built) {
        cnt = 0;
        for (idx_t j = 1; j < 12; ++j) {
            seq_t numerator = (seq_t)12 * (seq_t)j;
            seq_t denom     = (seq_t)(12 - (seq_t)j);
            if (numerator % denom == 0) {
                seq[cnt++] = numerator / denom;
            }
        }
        built = true;
    }

    if (n >= 1 && n <= cnt) {
        return seq[n - 1];
    } else {
        /* The sequence is finite; return 0 (or any convention) for n > 7. */
        return (seq_t)0;
    }
}
