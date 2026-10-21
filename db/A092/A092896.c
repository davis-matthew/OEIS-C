/*
 * A092896  Related to random walks on the 4-cube.
 * https://oeis.org/A092896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1; a(n)=4^(n-1)+1 for n>=2. The sequence satisfies
 *            a(n+1)=4^n+1 for n>=1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Handle the two base cases. */
    if (n <= 1)
        return (seq_t)1;

    /* Compute 4^(n-1).  Use a loop so that the result fits in seq_t
       (which may be a fixed-width or arbitrary-precision integer
       depending on the build). */
    seq_t p = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        p = p * (seq_t)4;          /* multiply by 4 each step */

    return p + (seq_t)1;          /* 4^(n-1)+1 */
}
