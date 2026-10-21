/*
 * A090078  In binary expansion of n, reduce contiguous blocks of 0's to 0.
 * https://oeis.org/A090078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = value of the binary expansion of n after collapsing each
 *            contiguous block of zeros to a single zero
 * Range:     u8 n <= 340, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Determine number of bits in n */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        tmp >>= 1;
        bits++;
    }

    seq_t res = 0;
    bool prev_zero = false;

    for (idx_t k = bits; k > 0; --k) {
        idx_t pos = k - 1;
        bool bit = (n >> pos) & 1;

        if (bit) {
            res = (res << 1) | (seq_t)1;
            prev_zero = false;
        } else {
            if (!prev_zero) {
                res <<= 1;            /* add single zero bit */
                prev_zero = true;
            }
            /* else skip additional zeros in the run */
        }
    }

    return res;
}
