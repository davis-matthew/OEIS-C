/*
 * A184033  1/16 the number of (n+1) X 4 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 3*2^((n+1)/2) + 34 for odd n * a(n) =
 *            9*2^(n/2-1) + 9*2^(n-1) + 34 for even n * (offset 1, a(1)=49)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;           /* offset starts at 1; undefined below */
    }
    if ((n & 1ULL) == 0ULL) {            /* n even */
        seq_t pow_n1   = pow2(n - 1);
        seq_t pow_half = pow2((n >> 1) - 1);
        return (seq_t)9 * pow_half + (seq_t)9 * pow_n1 + (seq_t)34;
    } else {                              /* n odd */
        seq_t pow_n1   = pow2(n - 1);
        seq_t pow_half = pow2((n + 1) >> 1);
        return (seq_t)9 * pow_n1 + (seq_t)3 * pow_half + (seq_t)34;
    }
}
