/*
 * A024056  a(n) = 5^n - n^7.
 * https://oeis.org/A024056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 13, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* compute 5^n */
    seq_t pow5 = pow_seq((seq_t)5, (int)n);

    /* compute n^7 */
    seq_t pown = pow_seq((seq_t)n, 7);

    /* a(n) = 5^n - n^7 */
    return pow5 - pown;
}
