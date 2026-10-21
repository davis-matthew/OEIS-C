/*
 * A081135  5th binomial transform of (0,0,1,0,0,0, ...).
 * https://oeis.org/A081135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5^(n-2) * binomial(n,2) for n >= 0 with binomial(n,2) =
 *            n*(n-1)/2. The function returns 0 for n = 0 and 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the initial values */
    if (n < 2) return 0;

    /* Compute binomial(n,2) as (n*(n-1))/2, using seq_t for the product */
    seq_t bin = (seq_t)n * (seq_t)(n - 1) / 2;

    /* Compute 5^(n-2) by repeated multiplication */
    seq_t power = 1;
    for (idx_t i = 0; i < n - 2; ++i)
        power *= (seq_t)5;

    /* Return the product */
    return power * bin;
}
