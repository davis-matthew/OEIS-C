/*
 * A176818  a(n) = (3^(2*n+1) + 2^(n+2))/7.
 * https://oeis.org/A176818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^(2*n+1) + 2^(n+2))/7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p3 = pow3((idx_t)2 * n + (idx_t)1);
    seq_t p2 = pow2(n + (idx_t)2);
    return (p3 + p2) / (seq_t)7;
}
