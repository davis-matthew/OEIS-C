/*
 * A033176  Threes and ones.
 * https://oeis.org/A033176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, less
 * Formula:   a(n) = concatenation of groups of the form 3…31 (k copies of
 *            digit '3' followed by a '1'), for k = 1..n. This implementation
 *            builds the number digit by digit using the power-of-10 helper
 *            from the utilities
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined for this sequence, but return 0 for safety. */
    if (n == 0) return 0;

    seq_t result = 0;                  /* accumulated value */
    seq_t pow10sh = pow10(2);           /* 10^(k+1); start with k=1 → 10^2=100 */

    for (idx_t k = 1; k <= n; ++k) {
        /* group_k = 33…31 (k times '3' then '1')
           value = (10^(k+1) - 7) / 3  */
        seq_t group = (pow10sh - 7) / 3;

        /* shift existing result left by (k+1) digits and append group */
        result = result * pow10sh + group;

        /* prepare shift for next iteration: 10^(k+2) */
        pow10sh *= 10;
    }

    return result;
}
