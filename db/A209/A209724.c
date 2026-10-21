/*
 * A209724  1/4 the number of (n+1) X 6 0..2 arrays with every 2 X 2 subblock
 *          having distinct clockwise edge differences.
 * https://oeis.org/A209724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 * 2^(n/2-1) + 6 for even n * a(n) = 2^((n+1)/2) + 6 for
 *            odd n * (The sequence is defined for n ≥ 1 with a(1)=8, a(2)=9,
 *            …)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is unsigned; no validation required for n ≥ 1 */
    seq_t val;

    if ((n & 1) == 0) {           /* even n */
        val = pow2(n / 2 - 1);    /* 2^(n/2-1) */
        val *= 3;                /* 3 * 2^(n/2-1) */
    } else {                      /* odd n */
        val = pow2((n + 1) / 2);  /* 2^((n+1)/2) */
    }
    val += 6;                     /* +6 for both cases */
    return val;
}
