/*
 * A023973  First bit in fractional part of binary expansion of 6th root of n.
 * https://oeis.org/A023973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(2*(n^(1/6) - floor(n^(1/6))))
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   int_root6, pow6_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is 0 if the first binary digit after the point of n^(1/6) is 0,
   1 otherwise. */
#include <stdint.h>
#include "../utils/int_root6.h"
#include "../utils/pow6_uint64.h"

/* main sequence function. */
seq_t a(idx_t n)
{
    uint64_t k = int_root6(n);                     /* floor(n^(1/6))          */
    unsigned __int128 left  = (unsigned __int128)n * 64;          /* 64*n   */
    unsigned __int128 right = pow6_uint64(2 * k + 1);             /* (2k+1)^6 */
    return (left >= right) ? (seq_t)1 : (seq_t)0;
}
