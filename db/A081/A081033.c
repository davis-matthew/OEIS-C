/*
 * A081033  6th binomial transform of the periodic sequence (1,7,1,1,7,1...).
 * https://oeis.org/A081033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*7^n - 3*5^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 273 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_idx.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Base case n == 0 */
    if (n == 0)
        return 1;               /* 4*1 - 3*1 = 1 */

    seq_t pow7 = ipow_idx((seq_t)7, n);
    seq_t pow5 = ipow_idx((seq_t)5, n);

    return 4 * pow7 - 3 * pow5;
}
