/*
 * A100740  Inverse modulo 2 binomial transform of 7^n.
 * https://oeis.org/A100740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = \prod_{i: n_i=1} (7^{2^i} - 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    idx_t k = n;
    while (k) {
        /* find the position of the lowest set bit */
        int shift = __builtin_ctzll(k);
        /* compute 7^{2^shift} */
        seq_t term = pow_seq((seq_t)7, (idx_t)1ULL << shift);
        term -= (seq_t)1;          /* 7^{2^shift} - 1 */
        result *= term;            /* accumulate product */
        k &= k - 1;                /* clear the lowest set bit */
    }
    return result;
}
