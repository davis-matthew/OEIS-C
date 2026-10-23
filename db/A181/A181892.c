/*
 * A181892  a(n) = (n!/4-1)^2.
 * https://oeis.org/A181892
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n!)/4-1)^2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    seq_t fact = factorial(n);      /* n! */
    seq_t y = fact / 4;             /* n!/4 */
    y -= 1;                         /* n!/4 - 1 */
    return y * y;                   /* square */
}
