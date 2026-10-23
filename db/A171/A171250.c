/*
 * A171250  Row lengths of A082381: number of iterations of "sum of digits
 *          squared" until 1 or 4 is reached.
 * https://oeis.org/A171250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, changed
 * Formula:   a(n) = number of iterations of "sum of digits squared" until 1
 *            or 4 is reached, applying the map at least once
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_square_sum.h"

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t steps = 0;

    do {
        /* compute sum of squares of decimal digits */
        x = digit_square_sum(x);
        steps++;
    } while (x != 1 && x != 4);

    return steps;
}
