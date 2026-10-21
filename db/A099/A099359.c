/*
 * A099359  a(n) = (2^n + 1)^3 - 2.
 * https://oeis.org/A099359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n + 1)^3 - 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   _pow_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_pow_seq_t.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2 = _pow_seq_t((seq_t)2, n);

    /* 2^n + 1 */
    seq_t tmp = pow2 + (seq_t)1;

    /* (2^n + 1)^2 */
    seq_t tmp2 = tmp * tmp;

    /* (2^n + 1)^3 */
    seq_t res = tmp2 * tmp;

    /* subtract 2 */
    res = res - (seq_t)2;

    return res;
}
