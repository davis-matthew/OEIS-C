/*
 * A173323  a(n) = 3*n! - 1.
 * https://oeis.org/A173323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * n! - 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t f = factorial(n);

    /* Multiply by 3 */
    f = f * (seq_t)3;

    /* Subtract 1 */
    f = f - (seq_t)1;

    return f;
}
