/*
 * A173862  a(n) = A158772(n-1)/21.
 * https://oeis.org/A173862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^floor((n-1)/3)
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 96, u64 n <= 192,
 *            u128 n <= 384, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1 – for n = 0 the value is undefined, return 0 to be safe */
    if (n == 0)
        return (seq_t)0;

    /* Compute floor((n-1)/3) using integer division */
    idx_t exp = (n - 1) / 3;
    return pow2(exp);
}
