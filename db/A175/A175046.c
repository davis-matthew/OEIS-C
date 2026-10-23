/*
 * A175046  Write n in binary, then increase each run of 0's by one 0, and
 *          increase each run of 1's by one 1. a(n) is the decimal equivalent
 *          of the result.
 * https://oeis.org/A175046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = interpret the binary representation of n with each run of
 *            0's lengthened by one 0 and each run of 1's lengthened by one 1,
 *            then treated as a binary number
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t result = 0;

    /* find position of the most significant bit set in n */
    idx_t pos = 63;
    while (pos > 0 && !(n & (1ULL << pos)))
        --pos;

    idx_t i = pos;
    while (1) {
        unsigned bit = (unsigned)((n >> i) & 1ULL);

        /* count run length */
        idx_t run = 1;
        while (i > 0 && ((n >> (i - 1)) & 1ULL) == bit) {
            ++run;
            --i;
        }

        /* output the run increased by one bit */
        for (idx_t k = 0; k <= run; ++k) {
            result = (result << 1) | (seq_t)bit;
        }

        if (i == 0)
            break;          /* finished the lowest bit */
        --i;                /* move to the next lower bit after the run */
    }

    return result;
}
