/*
 * A187386  a(n) = floor(s*n), where s=1+sqrt(3)-sqrt(2); complement of
 *          A187385.
 * https://oeis.org/A187386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((1+sqrt(3)-sqrt(2))*n)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 2, a(3) = 3, ... (Beatty sequence for s = 1+√3-√2) */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Offset: the sequence is defined for n >= 1. For n == 0 return 0. */
    if (n == 0)
        return 0;

    /* Use integer arithmetic throughout.  Interpret n as seq_t. */
    seq_t sq = (seq_t)n * (seq_t)n;                 /* n² */
    seq_t A  = floor_sqrt((seq_t)3 * sq);           /* floor(√3·n) */
    seq_t B  = floor_sqrt((seq_t)2 * sq);           /* floor(√2·n) */

    /* Remainders from the square identities */
    seq_t delta3 = (seq_t)3 * sq - A * A;           /* 3n² – A²  > 0 */
    seq_t delta2 = (seq_t)2 * sq - B * B;           /* 2n² – B²  > 0 */

    /* carry = 1 iff (δ3·B) < (δ2·A), i.e. fractional part of √3·n
       is smaller than that of √2·n. */
    seq_t carry = (delta3 * B < delta2 * A) ? 1 : 0;

    seq_t diff   = A - B;                           /* A – B = floor(√3·n) – floor(√2·n) */
    seq_t t_floor = diff - carry;                   /* floor((√3-√2)·n) */

    return (seq_t)n + t_floor;                     /* a(n) = floor((1+√3-√2)·n) */
}
