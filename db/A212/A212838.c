/*
 * A212838  Number of 0..n arrays of length 5 with 0 never adjacent to n.
 * https://oeis.org/A212838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 2*n^3 - 8*n^2 + n + 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n */
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t n4 = n3 * nn;       /* n^4 */
    seq_t n5 = n4 * nn;       /* n^5 */

    /* Evaluate the polynomial */
    seq_t val = n5
               + (seq_t)5 * n4
               + (seq_t)2 * n3
               - (seq_t)8 * n2
               + nn
               + (seq_t)1;

    return val;
}
