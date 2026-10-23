/*
 * A195391  a(n) = (7*n)!.
 * https://oeis.org/A195391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the limit m = 7*n as idx_t. This is safe for n up to at least 10000
       and covers the typical use cases for the sequence. */
    idx_t m = n * (idx_t)7;

    /* Start with 1, then multiply successively by the integers 2 .. m. */
    seq_t result = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        result *= (seq_t)i;

    return result;
}
