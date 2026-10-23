/*
 * A047460  Numbers that are congruent to {0, 1, 3, 4} mod 8.
 * https://oeis.org/A047460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*((n-1)/4) + offset[(n-1)%4] offset[0]=0, offset[1]=1,
 *            offset[2]=3, offset[3]=4 (Numbers congruent to {0,1,3,4} mod 8)
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Watch out for the unofficial offset 0; the sequence is defined
       for n >= 1. If n == 0, return 0 to avoid underflow. */
    if (n == 0)
        return 0;

    /* Convert to zero‑based index k = n-1.  The pattern repeats every
       four numbers. */
    idx_t k      = n - 1;
    idx_t block  = k / 4;          /* complete 4‑item groups   */
    idx_t r      = k % 4;          /* position inside group    */

    /* Base value 8 * block, cast to seq_t to keep the result in the
       correct type.  The product is performed with idx_t arithmetic
       (they are unsigned), then converted. */
    seq_t base = (seq_t)(block * (idx_t)8);

    /* Static table of the four offsets for each remainder class. */
    static const seq_t offs[4] = {0, 1, 3, 4};

    /* Add the appropriate offset.  r is small (0..3) so a cast to
       unsigned int is safe.  Indexing a static array is allowed. */
    return base + offs[(unsigned int)r];
}
