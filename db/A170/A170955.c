/*
 * A170955  a(n) = 10^n - 9.
 * https://oeis.org/A170955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 9, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Since the sequence is defined for n >= 1, we assume n >= 1.
       For n == 0 the behaviour is undefined. */
    seq_t result = pow10(n);       /* compute 10^n in seq_t */
    result -= (seq_t)9;            /* subtract 9 */
    return result;
}
