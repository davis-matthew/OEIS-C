/*
 * A017132  a(n) = (8*n + 5)^8.
 * https://oeis.org/A017132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 31, u128 n <= 8191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t base = (seq_t)8 * n + 5;   /* base = 8*n + 5 */
    seq_t b2  = base * base;        /* (8n+5)^2 */
    seq_t b4  = b2 * b2;            /* (8n+5)^4 */
    return b4 * b4;                 /* (8n+5)^8 */
}
