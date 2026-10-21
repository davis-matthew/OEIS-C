/*
 * A200322  Related to numbers written in base 2 (see below for definition).
 * https://oeis.org/A200322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..floor(log2 n)} [ first k+1 bits of n == last
 *            k+1 bits of n ] * 2^k
 * Range:     u8 n <= 255, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special case: a(0) = 1 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the unique case n == 0 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Compute m = floor(log2 n)  (bit length - 1) */
    idx_t m = 0;
    for (idx_t tmp = n; tmp >> 1; ++m)
        tmp >>= 1;
    /* After loop, m is the position of the highest set bit */

    seq_t result = 0;

    /* For each k from 0 to m inclusive */
    for (idx_t k = 0; k <= m; ++k)
    {
        /* suffix: last k+1 bits of n */
        idx_t suffix_mask;
        if ((k + 1) == (sizeof(idx_t) * 8))
            suffix_mask = (idx_t)-1;        /* all bits set */
        else
            suffix_mask = ((idx_t)1 << (k + 1)) - 1ULL;
        idx_t suffix = n & suffix_mask;

        /* prefix: first k+1 bits of n */
        idx_t shift = m - k;
        idx_t prefix = n >> shift;          /* top k+1 bits */

        if (prefix == suffix)
        {
            /* Set bit k in the result (use idx_t for the shift) */
            idx_t bit = (idx_t)1 << k;
            result |= (seq_t)(bit);
        }
    }

    return result;
}
