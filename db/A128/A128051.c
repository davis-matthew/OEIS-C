/*
 * A128051  a(n) = (n^3 + n)*7^n.
 * https://oeis.org/A128051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + n)*7^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* Compute n^2 and n^3 */
    seq_t N2 = N * N;      /* n^2 */
    seq_t N3 = N2 * N;     /* n^3 */

    /* Compute n^3 + n */
    seq_t sum = N3 + N;

    /* Compute 7^n */
    seq_t pow7 = pow_int((seq_t)7, n);

    /* Return the product */
    return sum * pow7;
}
