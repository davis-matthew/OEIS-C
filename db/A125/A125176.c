/*
 * A125176  Row sums of A125175.
 * https://oeis.org/A125176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=3, a(n)=7*2^(n-3) for n>=3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) {
        /* The sequence is defined for n >= 1; the value for 0 is undefined.
           Returning 0 does not affect correct calls for n >= 1. */
        return (seq_t)0;
    }
    if (n == (idx_t)1) {
        return (seq_t)1;
    }
    if (n == (idx_t)2) {
        return (seq_t)3;
    }
    /* For n >= 3: a(n) = 7 * 2^(n-3) */
    return ((seq_t)7) * pow2(n - (idx_t)3);
}
