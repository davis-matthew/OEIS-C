/*
 * A100167  Structured pentagonal icositetrahedral numbers (vertex structure
 *          13).
 * https://oeis.org/A100167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (60*n^3 - 72*n^2 + 18*n)/6 (simplifies to 10*n^3 - 12*n^2
 *            + 3*n)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Cast n to the sequence type first to keep arithmetic in seq_t */
    seq_t nn   = (seq_t)n;      /* n */
    seq_t n2   = nn * nn;       /* n^2 */
    seq_t n3   = n2 * nn;       /* n^3 */

    /* Compute 10*n^3 + 3*n first, then subtract 12*n^2.
       This order avoids unsigned underflow on small seq_t types. */
    seq_t term = 10 * n3 + 3 * nn;
    term -= 12 * n2;

    return term;
}
