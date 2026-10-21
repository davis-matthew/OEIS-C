/*
 * A236773  a(n) = n + floor( n^2/2 + n^3/3 ).
 * https://oeis.org/A236773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n^2)/2 + (n^3)/3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;          /* convert index to seq_t */
    seq_t n2 = n_t * n_t;          /* n^2 */
    seq_t n3 = n2 * n_t;           /* n^3 */
    seq_t part = (n2 * 3 + n3 * 2) / 6; /* floor((n^2)/2 + (n^3)/3) */
    return n_t + part;
}
