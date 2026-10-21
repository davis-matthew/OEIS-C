/*
 * A060892  a(n) = n^8 - n^6 + n^4 - n^2 + 1.
 * https://oeis.org/A060892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^8 - n^6 + n^4 - n^2 + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 256, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to the sequence type */
    seq_t x  = (seq_t)n;
    /* Compute powers step by step to avoid intermediate overflow in builds
     * where seq_t is an arbitrary-precision integer class. */
    seq_t n2 = x * x;          /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n6 = n4 * n2;        /* n^6 */
    seq_t n8 = n4 * n4;        /* n^8 */

    return n8 - n6 + n4 - n2 + 1;
}
