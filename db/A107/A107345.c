/*
 * A107345  From the binary representation of n: binomial(number of zeros,
 *          number of blocks of contiguous zeros).
 * https://oeis.org/A107345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(number_of_zeros_in_binary(n),
 *            number_of_zero_blocks_in_binary(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case n == 0 : binary representation is "0" */
    if (n == 0) {
        return (seq_t)1;   /* binomial(1,1) = 1 */
    }

    idx_t zeros = 0;
    idx_t blocks = 0;
    bool prevZero = false;

    /* Find number of bits in n (position of most significant bit + 1) */
    idx_t msb = 0;
    for (int i = 63; i >= 0; --i) {
        if ((n >> i) & (idx_t)1) {
            msb = (idx_t)i + 1;
            break;
        }
    }

    /* Scan bits from most significant to least significant */
    for (idx_t i = msb; i > 0; --i) {
        bool bit = (bool)((n >> (i - 1)) & (idx_t)1);
        if (!bit) {                 /* current bit is 0 */
            ++zeros;
            if (!prevZero) {        /* start of a new block of zeros */
                ++blocks;
                prevZero = true;
            }
        } else {                     /* current bit is 1 */
            prevZero = false;
        }
    }

    return binomial(zeros, blocks);
}
