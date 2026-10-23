/*
 * A048487  a(n) = T(4,n), array T given by A048483.
 * https://oeis.org/A048487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - 4
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t val = pow_seq((seq_t)2, (int)n);
    /* 5*2^n */
    val = val * 5;
    /* 5*2^n - 4 */
    val -= (seq_t)4;
    return val;
}
