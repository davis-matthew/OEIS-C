/*
 * A106151  In binary representation of n: delete one zero in each contiguous
 *          block of zeros.
 * https://oeis.org/A106151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the binary representation of n with one zero removed from
 *            each block of consecutive zeros. Implementation: scan bits from
 *            most significant to least, skipping the first zero of every zero
 *            block
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)n;

    /* Determine the number of bits in n */
    idx_t len = 0;
    idx_t temp = n;
    while (temp) {
        ++len;
        temp >>= 1;
    }

    seq_t result = 0;
    bool inZeroBlock = false;

    for (idx_t mask = ((idx_t)1) << (len - 1); mask; mask >>= 1) {
        if (n & mask) {
            /* Bit 1 – keep it */
            result = (result << 1) | (seq_t)1;
            inZeroBlock = false;
        } else {
            if (!inZeroBlock) {
                /* First zero of a block – skip it */
                inZeroBlock = true;
            } else {
                /* Subsequent zeros in the same block – keep them */
                result = (result << 1);
            }
        }
    }

    return result;
}
