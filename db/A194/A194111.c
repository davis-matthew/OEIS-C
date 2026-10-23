/*
 * A194111  Sum{floor(j*sqrt(7)) : 1<=j<=n}; n-th partial sum of Beatty
 *          sequence for sqrt(7).
 * https://oeis.org/A194111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{j=1}^{n} \lfloor j\sqrt{7}\rfloor
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j)
    {
        seq_t jj  = (seq_t)j;
        seq_t val = floor_sqrt((seq_t)7 * jj * jj);
        sum += val;
    }
    return sum;
}
