/*
 * A039623  a(n) = n^2*(n^2+3)/4.
 * https://oeis.org/A039623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^2 * (n^2 + 3) / 4
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 342, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For safety we return 0 for n = 0. */
    if (n == 0) return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t square = nn * nn;          /* n^2          */
    seq_t term   = square + (seq_t)3;/* n^2 + 3      */

    /* Divide one of the two factors by 2 first to keep the
       intermediate product small and avoid overflow on fixed-width types. */
    if ((square & (seq_t)1) != 0) {
        term /= (seq_t)2;
    } else {
        square /= (seq_t)2;
    }

    seq_t product = square * term;   /* (n^2)/2 * (n^2+3)/2  (or swapped) */
    product /= (seq_t)2;             /* last division by 2     */

    return product;
}
