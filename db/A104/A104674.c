/*
 * A104674  a(n) = binomial(n+6, 6) * binomial(n+11, n).
 * https://oeis.org/A104674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+6,6) * binomial(n+11,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 51, u128 n <= 756,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 997 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t part1 = binomial(n + 6, 6);   /* C(n+6,6)  */
    seq_t part2 = binomial(n + 11, 11); /* C(n+11,11) = C(n+11,n)  */
    return part1 * part2;
}
