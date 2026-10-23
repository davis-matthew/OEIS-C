/*
 * A135576  Numbers whose binary expansion has only the digit "1" as first,
 *          central and final digit.
 * https://oeis.org/A135576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1; for n>1 a(n) = 2^(2(n-1)) + 2^(n-1) + 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 0)
        return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n == 1)
        return (seq_t)1;          /* first term */
    idx_t k = n - 1;               /* offset for the formula */
    seq_t high = pow2(2 * k);      /* 2^(2k) */
    seq_t mid  = pow2(k);          /* 2^k */
    return high + mid + (seq_t)1;  /* 2^(2k) + 2^k + 1 */
}
