/*
 * A036576  a(n) is the least number not of the form floor(k^2/n).
 * https://oeis.org/A036576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((m + n + 2*sqrt(m*n - 1))/4) where m = ((n-1) mod
 *            4) + 1; a(1)=2
 * Range:     u8 n <= 19, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A036576 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)2;
    }

    /* compute m = ((n-1) mod 4) + 1 */
    idx_t nn = n - 1u;
    idx_t mm = (nn % 4u) + 1u;
    seq_t m = (seq_t)mm;

    /* temp = m * n - 1 */
    seq_t temp = m * (seq_t)n - (seq_t)1;

    /* sqrt_val = floor(sqrt(temp)) */
    seq_t sqrt_val = floor_sqrt(temp);

    /* numerator = m + n + 2 * sqrt_val */
    seq_t numerator = m + (seq_t)n + (seq_t)2 * sqrt_val;

    /* result = floor(numerator / 4) */
    seq_t result = numerator / (seq_t)4;

    return result;
}
