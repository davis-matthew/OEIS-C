/*
 * A052146  a(n) = floor((sqrt(1+8*n)-3)/2).
 * https://oeis.org/A052146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((sqrt(1+8*n)-3)/2)
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

/* Compute A052146: a(n) = floor((sqrt(1+8*n)-3)/2) for n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* index to sequence type */
    seq_t temp = nn * (seq_t)8 + (seq_t)1;  /* 1 + 8*n  */
    seq_t s    = isqrt_seq(temp);   /* floor(sqrt(1+8*n)) */
    seq_t r    = (s - (seq_t)3) / (seq_t)2; /* (s-3)/2 */
    return r;
}
