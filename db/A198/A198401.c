/*
 * A198401  a(n) = 8^n + n^8.
 * https://oeis.org/A198401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n + n^8
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a sequence term a(n) = 8^n + n^8.
 * seq_t is the sequence element type (uint8_t, ..., unsigned __int128,
 * or arbitrary‑precision integer). idx_t is an unsigned integer type
 * (uint64_t).  The function works for any n >= 0 and uses only
 * integer arithmetic. */
seq_t a(idx_t n)
{
    /* compute 8^n by repeated multiplication */
    seq_t pow8 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow8 *= (seq_t)8;

    /* compute n^8 by repeated squaring (constant work) */
    seq_t n0 = (seq_t)n;
    seq_t n2 = n0 * n0;   /* n^2 */
    seq_t n4 = n2 * n2;   /* n^4 */
    seq_t n8 = n4 * n4;   /* n^8 */

    /* return the sum */
    return pow8 + n8;
}
