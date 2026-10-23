/*
 * A186339  a(n) = A006125(n+1) * 2^A001840(n).
 * https://oeis.org/A186339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^binomial(n+1,2) * 2^floor((n+1)*(n+2)/6) = 2^E where E
 *            = binomial(n+1,2) + floor((n+1)*(n+2)/6)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute binomial(n+1,2) = (n+1)*n/2 */
    idx_t nn = n + 1;
    idx_t bin = nn * (nn - 1) / 2;

    /* Compute floor((n+1)*(n+2)/6) */
    idx_t tri = nn * (nn + 1) / 6;

    idx_t exp = bin + tri;
    return pow2(exp);
}
