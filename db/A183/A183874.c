/*
 * A183874  a(n) = n + floor(sqrt(3*n/2)). Complement of A183857.
 * https://oeis.org/A183874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(3*n/2))
 * Range:     u8 n <= 42, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    seq_t t  = (seq_t)3 * ns / (seq_t)2;   /* t = floor(3*n/2) */
    seq_t s  = floor_sqrt(t);              /* s = floor(sqrt(t)) */
    return ns + s;
}
