/*
 * A173923  In the sequence of nonnegative integers substitute all n by
 *          2^floor(n/8) occurrences of (n mod 2).
 * https://oeis.org/A173923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the parity of the integer m whose block covers *
 *            position n. Each integer m contributes 2^floor(m/8) copies of *
 *            m mod 2 in the sequence. The computation is done in O(log n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <limits.h>   /* for ULLONG_MAX */

seq_t a(idx_t n)
{
    idx_t rem   = n;   /* remaining offset within the current group   */
    idx_t q     = 0;   /* group index (floor(m/8))                    */
    idx_t block = 1;   /* current block size = 2^q                   */

    /* Walk through whole groups until the current group contains n */
    while (1) {
        /* avoid overflow when computing 8*block                     */
        if (block > ULLONG_MAX / 8)
            break;                           /* remaining groups are too big */

        idx_t group = block << 3;             /* 8 * block (= total positions in this group) */

        if (rem < group)          /* n lies inside this group */
            break;

        rem   -= group;           /* skip this whole group            */
        block <<= 1;              /* next group has twice the block size */
        q++;                      /* advance group index             */
    }

    idx_t k = rem / block;         /* index of the block within group   */
    idx_t m = (q * 8) + k;          /* the integer that covers position n */

    return (seq_t)(m & 1);         /* parity of that integer           */
}
