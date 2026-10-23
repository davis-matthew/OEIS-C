/*
 * A147874  a(n) = (5*n-7)*(n-1).
 * https://oeis.org/A147874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n-7)*(n-1) (offset 1)
 * Range:     u8 n <= 8, u16 n <= 115, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;            /* convert index to seq_t before arithmetic */
    return ( (5 * N - 7) * (N - 1) ); /* direct evaluation of the closed form */
}
