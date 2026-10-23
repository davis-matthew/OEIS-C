/*
 * A184037  1/16 the number of (n+1) X 8 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 3*2^((n+1)/2) + 610 for n odd; a(n) =
 *            9*2^(n/2-1) + 9*2^(n-1) + 610 for n even
 * Range:     u8 none, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((n & 1) == 0) {
        return 9 * pow2(n / 2 - 1) + 9 * pow2(n - 1) + 610;
    } else {
        return 9 * pow2(n - 1) + 3 * pow2((n + 1) / 2) + 610;
    }
}
