/*
 * A220161  a(n) = 1 + 2^(2^n) + 2^(2^(n+1)).
 * https://oeis.org/A220161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 2^(2^n) + 2^(2^(n+1))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   _pow2_of_two_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow2_of_two_exp.h"

seq_t a(idx_t n)
{
    seq_t b = _pow2_of_two_exp(n);        /* b = 2^(2^n)   */
    seq_t c = b * b;                      /* c = 2^(2^(n+1)) = b^2 */
    return 1 + b + c;                     /* a(n) = 1 + b + c */
}
