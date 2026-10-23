/*
 * A173873  a(n) = 2*a(n-1) + 13, a(1)=1.
 * https://oeis.org/A173873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*2^n-13 (a(1)=1, a(n)=2*a(n-1)+13)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence satisfies a(n) = 7*2^n - 13 for n >= 1. */
    seq_t val = pow2(n);          /* val = 2^n */
    val = val * (seq_t)7;         /* val = 7*2^n */
    val = val - (seq_t)13;        /* val = 7*2^n - 13 */
    return val;
}
