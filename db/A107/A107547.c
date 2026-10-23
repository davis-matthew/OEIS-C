/*
 * A107547  Coordination sequence for B_14 lattice.
 * https://oeis.org/A107547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0}^{min(n,14)} c_k * binom(n + 13 - k, 13) where
 *            the coefficients c_k are those of the numerator of the G.f.: c_0
 *            = 1, c_1 = 378, c_2 = 23387, c_3 = 472836, c_4 = 4284137, c_5 =
 *            20009990, c_6 = 51859899, c_7 = 77510712, c_8 = 67815867, c_9 =
 *            34561254, c_10 = 9994985, c_11 = 1552772,...
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 6, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    static const uint32_t coef[15] = {
        1,        378,      23387,      472836,      4284137,
        20009990, 51859899, 77510712,  67815867,   34561254,
        9994985,  1552772,  116571,     3290,        1
    };

    seq_t sum = 0;
    idx_t maxk = n < 14 ? n : 14;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        seq_t bin = binom(n + 13 - k, 13);
        sum += coef[k] * bin;
    }
    return sum;
}
