/*
 * A081104  Duplicate of A079028.
 * https://oeis.org/A081104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = (n==1?1:4^(n-2)*(n+3))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 12, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t val = 1;
    val <<= (unsigned int)(2 * (n - 2));   /* 4^(n-2) = 2^(2*(n-2)) */
    return val * ((seq_t)n + 3);
}
