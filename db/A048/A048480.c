/*
 * A048480  a(n) = T(8,n), array T given by A048472.
 * https://oeis.org/A048480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n*2^(n-1)+1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 118,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow = pow2(n - 1);                 /* 2^(n-1) */
    seq_t res = (seq_t)9 * n * pow + (seq_t)1;
    return res;
}
