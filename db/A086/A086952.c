/*
 * A086952  a(n) = n^2*4^n/4.
 * https://oeis.org/A086952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * 4^(n-1) (for n >= 1, a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation avoids division by 4 by exponentiating 4 to the (n-1)-th power. */

#include <stdbool.h>  /* for bool, but not strictly needed */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute n^2 */
    seq_t sq = (seq_t)n;
    sq *= sq;                /* sq = n^2 */

    /* Compute 4^(n-1) by repeated multiplication */
    seq_t pow4 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)   /* n-1 iterations */
        pow4 *= (seq_t)4;           /* pow4 = pow4 * 4 */

    /* Result: n^2 * 4^(n-1) */
    return sq * pow4;
}
