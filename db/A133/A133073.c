/*
 * A133073  a(n) = n^5 + n^3 + n^2.
 * https://oeis.org/A133073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + n^3 + n^2
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) = n^5 + n^3 + n^2 for the OEIS sequence A133073. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast to seq_t first */
    seq_t s2 = sn * sn;           /* n^2 */
    seq_t s3 = s2 * sn;           /* n^3 */
    seq_t s5 = s3 * s2;           /* n^5 (n^3 * n^2) */
    return s5 + s3 + s2;          /* n^5 + n^3 + n^2 */
}
