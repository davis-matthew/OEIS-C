/*
 * A160927  a(n) = n + reversal(n-1).
 * https://oeis.org/A160927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + rev(n-1)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* rev is decimal digit reversal of an unsigned integer */

#include "../utils/rev.h"

seq_t a(idx_t n)
{
    if (n == 0u)   /* not part of the defined range, but safe guard */
        return 0;

    /* compute reverse of n-1 */
    idx_t rev_val = rev(n - 1u);

    /* add to n, casting to seq_t for full integer precision */
    return (seq_t)n + (seq_t)rev_val;
}
