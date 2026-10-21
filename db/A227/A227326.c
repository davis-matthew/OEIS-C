/*
 * A227326  a(2n) = 2a(n)+2^(2n), a(2n+1) = 2^(2n+1), a(0)=0.
 * https://oeis.org/A227326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   a(2n)   = 2*a(n) + 2^(2n);
   a(2n+1) = 2^(2n+1)          (equivalently a(n)=2^n for odd n) */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n & 1U)        /* odd index: a(n) = 2^n */
        return pow2(n);
    else {             /* even index: a(n) = 2*a(n/2) + 2^n */
        idx_t half = n >> 1;
        seq_t prev = a(half);
        return (prev << 1) + pow2(n);
    }
}
