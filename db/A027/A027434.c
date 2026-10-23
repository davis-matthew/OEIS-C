/*
 * A027434  a(1) = 2; then defined by property that a(n) = smallest number >=
 *          a(n-1) such that successive runs have lengths 1,1,2,2,3,3,4,4.
 * https://oeis.org/A027434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 1 + floor( sqrt(4*n - 3) )
 * Range:     u8 n <= 16, u16 n <= 3781, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined by
          a(n) = 1 + ⌊√(4n - 3)⌋
       for n >= 1. This formula gives the exact term
       without any search or recursion and works for any
       value of n that fits in idx_t. */
    seq_t tn = (seq_t)n;          /* cast index to seq_t */
    seq_t expr = tn * 4 - 3;      /* 4*n - 3, safe because tn is seq_t */
    seq_t sq = floor_sqrt(expr);  /* integer part of the square root */
    return sq + 1;
}
