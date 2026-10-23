/*
 * A184044  1/9 the number of (n+1) X 6 0..2 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2) + 2^(n+1) + 73 for n even; * a(n) = 2^(n+1) +
 *            2^((n+3)/2) + 73 for n odd
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    const seq_t two = 2;
    const seq_t const73 = 73;
    seq_t pow2_nplus1 = big_pow(two, (seq_t)(n + 1));

    if ((n & 1) == 0) {                /* n is even */
        seq_t half      = n / 2;
        seq_t pow2_half = big_pow(two, (seq_t)half);
        return pow2_nplus1 + (seq_t)3 * pow2_half + const73;
    } else {                            /* n is odd */
        seq_t halfplus      = (n + 3) / 2;
        seq_t pow2_halfplus = big_pow(two, (seq_t)halfplus);
        return pow2_nplus1 + pow2_halfplus + const73;
    }
}
