/*
 * A226731  a(n) = (2n - 1)!/(2n).
 * https://oeis.org/A226731
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2n - 1)!/(2n) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* values below the offset are irrelevant */

    idx_t limit = 2 * n - 1;      /* compute up to (2n-1) */
    seq_t denom = (seq_t)2 * (seq_t)n; /* denominator 2n */
    seq_t result = 1;

    for (idx_t k = 1; k <= limit; ++k)
    {
        seq_t g   = gcd((seq_t)k, denom);   /* cancel common factors first */
        denom     = denom / g;
        seq_t mul = (seq_t)k / g;
        result *= mul;
    }

    /* after the loop, denom should be 1; keep defensive */
    if (denom > 1)
        result /= denom;

    return result;
}
