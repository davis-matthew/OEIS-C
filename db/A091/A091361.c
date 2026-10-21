/*
 * A091361  Numbers n such that A001840(n) == 0 (mod n).
 * https://oeis.org/A091361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n, for n = 1,2 a(n) = 6*n - 15, for n >= 3 This matches
 *            the OEIS definition: the numbers n such that A001840(n) is
 *            divisible by n are 1, 2, and all integers congruent to 3 mod 6
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;          /* convert index to seq_t */
    if (n_seq < 3) return n_seq;      /* a(1)=1, a(2)=2 */
    return n_seq * 6 - (seq_t)15;     /* a(n)=6*n-15 for n>=3 */
}
