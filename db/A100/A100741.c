/*
 * A100741  Inverse modulo 2 binomial transform of 8^n.
 * https://oeis.org/A100741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{i∈bits(n)} (8^{2^i} - 1) (Empty product equals 1 for n
 *            = 0. The product is taken over all set bits of the binary
 *            representation of n.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  result   - running product of (8^{2^i} - 1)   */
    /*  power    - current value of 8^{2^i}          */
    seq_t result = (seq_t)1;
    seq_t power  = (seq_t)8;           /* 8^{2^0}  */
    idx_t m = n;

    while (m) {
        if (m & (idx_t)1) {
            /* multiply by (power - 1) for this set bit   */
            seq_t mult = power - (seq_t)1;
            result *= mult;
        }
        /* move to next bit: square the power for 8^{2^{i+1}} */
        power *= power;
        m >>= 1;
    }
    return result;
}
