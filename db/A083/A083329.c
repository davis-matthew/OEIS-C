/*
 * A083329  a(0) = 1; for n > 0, a(n) = 3*2^(n-1) - 1.
 * https://oeis.org/A083329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = 1, if n = 0; otherwise a(n) = 3*2^(n-1) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 2^(n-1) */
    seq_t p = pow2(n - 1);

    /* Compute 3 * 2^(n-1) - 1 */
    p = (seq_t)3 * p;
    p -= (seq_t)1;

    return p;
}
