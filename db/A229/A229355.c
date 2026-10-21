/*
 * A229355  Total sum of n-th powers of parts in all partitions of 4.
 * https://oeis.org/A229355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 + 3*2^n + 3^n + 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t p2 = pow2(n);
    seq_t p3 = pow3(n);
    seq_t p4 = pow4(n);
    seq_t result = (seq_t)7 + (seq_t)3 * p2 + p3 + p4;
    return result;
}
