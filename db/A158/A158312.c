/*
 * A158312  a(n) = 400*n^2 + 2*n.
 * https://oeis.org/A158312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 400*n^2 + 2*n
 * Range:     u8 none, u16 n <= 12, u32 n <= 3276, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  For n = 0 the value is undefined,
     * but we return 0 for safety.  The computation is done in seq_t to
     * avoid overflow on the intermediate and final results. */
    if (n == 0)
        return 0;
    seq_t N = (seq_t)n;
    return N * ((seq_t)400 * N + (seq_t)2);
}
