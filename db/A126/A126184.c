/*
 * A126184  Number of hex trees with n edges and having no nonroot nodes of
 *          outdegree 2.
 * https://oeis.org/A126184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(1) = 3; a(n) = (n+8)*3^(n-2) for n >= 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* For n >= 2, compute 3^(n-2) and multiply by (n+8) */
    seq_t pow_val = pow_uint(3, n-2);          /* 3^(n-2) */
    seq_t factor = (seq_t)(n + 8);             /* n+8 */
    return factor * pow_val;
}
