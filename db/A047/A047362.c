/*
 * A047362  Numbers that are congruent to {2, 3, 4, 5} mod 7.
 * https://oeis.org/A047362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 1 + 3*floor((n-1)/4)
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n >= 1 this formula gives the nth
       integer congruent to 2,3,4,5 modulo 7. */
    if (n == 0)
        return (seq_t)0;                       /* undefined by offset, safe return */

    idx_t k = (n - 1) / 4;                     /* floor((n-1)/4) using uint64_t */
    seq_t N = (seq_t)n;                        /* cast to output type */
    return N + 1 + (seq_t)3 * (seq_t)k;
}
