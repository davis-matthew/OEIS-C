/*
 * A093039  Sequence resulting from a sum of three repeated binomial(n+3,4)
 *          sequences.
 * https://oeis.org/A093039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(n)+b(n-1)+b(n-2) for n>2, where b(n) =
 *            binomial(floor((n+7)/2), 4). Initial values: a(1)=1, a(2)=2
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   b_value, binom_choose4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/b_value.h"
#include "../utils/binom_choose4.h"

seq_t a(idx_t n)
{
    if (n == 1) return b_value(1);
    if (n == 2) return b_value(1) + b_value(2);
    return b_value(n) + b_value(n - 1) + b_value(n - 2);
}
