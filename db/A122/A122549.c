/*
 * A122549  From item 6 of Cooijman's Odds test (see example line for an
 *          explanation).
 * https://oeis.org/A122549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   A122549: For k≥0,
     a(3k+1) = 5*(k+2)*(k+3) – 11
     a(3k+2) = digit_sum(a(3k+1))
     a(3k+3) = a(3k+1) – a(3k+2)
   where digit_sum(x) is the sum of the decimal digits of x.
*/
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    idx_t k = (n - 1) / 3;
    seq_t temp = (seq_t)(k + 2) * (seq_t)(k + 3);
    seq_t first = (seq_t)5 * temp - (seq_t)11;
    switch (n % 3) {
    case 1:
        return first;
    case 2:
        return digit_sum(first);
    case 0:
        return first - digit_sum(first);
    default:
        return (seq_t)0; /* unreachable */
    }
}
