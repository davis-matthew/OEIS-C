/*
 * A078876  a(n) = n^4*(n^4-1)/240.
 * https://oeis.org/A078876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^4-1)/240
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t n_seq = (seq_t)n;

    /* Compute n^2 and n^4 */
    seq_t n2 = n_seq * n_seq;           /* n^2 */
    seq_t n4 = n2 * n2;                 /* n^4 */

    /* Compute numerator n^4 * (n^4 - 1) */
    seq_t numerator = n4 * (n4 - (seq_t)1);

    /* Divide by 240 (guaranteed to divide evenly) */
    return numerator / (seq_t)240;
}
