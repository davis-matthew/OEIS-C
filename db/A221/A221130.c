/*
 * A221130  a(n) = 2^(2*n - 1) + n.
 * https://oeis.org/A221130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n - 1) + n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behaviour for n==0 is
       unspecified but the function will simply return 0. */
    if (n == 0)
        return 0;
    seq_t val = pow2(2 * n - 1);
    val += (seq_t)n;
    return val;
}
