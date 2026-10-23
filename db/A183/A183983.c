/*
 * A183983  1/4 the number of (n+1) X 7 binary arrays with all 2 X 2 subblock
 *          sums the same.
 * https://oeis.org/A183983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2-1) + 2^(n-1) + 42 for n even, a(n) = 2^(n-1) +
 *            2^((n+1)/2) + 42 for n odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result;
    if ((n & 1) == 0) {                /* even n */
        seq_t half = pow2(n / 2 - 1);   /* 2^(n/2-1) */
        seq_t full = pow2(n - 1);       /* 2^(n-1)   */
        result = 3 * half + full + 42;
    } else {                            /* odd n */
        seq_t full = pow2(n - 1);       /* 2^(n-1)   */
        seq_t half = pow2((n + 1) / 2); /* 2^((n+1)/2) */
        result = full + half + 42;
    }
    return result;
}
