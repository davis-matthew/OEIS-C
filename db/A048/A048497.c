/*
 * A048497  a(n) = 2^(n-1)*(4*n-6) + 4.
 * https://oeis.org/A048497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(4*n-6) + 4 , n >= 1 * a(0) = 1 * The product
 *            (4*n-6) can be negative only for n = 1; * this case is handled
 *            separately to keep all arithmetic unsigned
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
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
    if (n == 1)
        return (seq_t)2;

    /* compute 2^(n-1) as a power of two */
    seq_t p = pow2(n - 1);

    /* multiplier (4*n-6) is non‑negative for n >= 2 */
    seq_t mult = (seq_t)(4 * (seq_t)n - (seq_t)6);

    return p * mult + (seq_t)4;
}
