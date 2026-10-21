/*
 * A221943  Floor(sqrt(7*2^n)).
 * https://oeis.org/A221943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(7*2^n))
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = pow2(n);      /* 2^n */
    val = (seq_t)7 * val;     /* 7*2^n */
    return floor_sqrt(val);   /* floor(sqrt(...)) */
}
