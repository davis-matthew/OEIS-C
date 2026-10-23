/*
 * A101052  Number of preferential arrangements of n labeled elements when
 *          only k <= 3 ranks are allowed.
 * https://oeis.org/A101052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 2 - 2*2^n (OEIS A101052)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    seq_t two = (seq_t)2;
    seq_t pow2 = pow_seq(two, n);          /* 2^n */
    seq_t pow3 = pow_seq((seq_t)3, n);     /* 3^n */
    seq_t result = pow3 + two - (two * pow2); /* 3^n + 2 - 2*2^n */
    return result;
}
