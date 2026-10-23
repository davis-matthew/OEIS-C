/*
 * A048506  a(n) = T(0,n), array T given by A048505.
 * https://oeis.org/A048506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*2^(n-2)+1 (for n >= 2; a(0)=1, a(1)=2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 54, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    return (seq_t)n * ((seq_t)n + 1) * pow2(n - 2) + 1;
}
