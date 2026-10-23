/*
 * A019521  Concatenate squares.
 * https://oeis.org/A019521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = concat_{i=1}^{n} (i^2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"   /* seq_t pow10(idx_t e); */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        /* compute i^2 as seq_t */
        seq_t sq = (seq_t)i * (seq_t)i;

        /* count decimal digits of sq using generic division */
        seq_t temp = sq;
        int d = 0;
        do {
            ++d;
            temp /= 10;
        } while (temp);

        /* shift result by d decimal places and append sq */
        result = result * pow10(d) + sq;
    }
    return result;
}
