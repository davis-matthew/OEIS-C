/*
 * A021239  Decimal expansion of 1/235.
 * https://oeis.org/A021239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 235) mod 10 (nth digit after decimal
 *            point of 1/235, 0‑indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t DEN = 235;
    uint64_t r = 1;          /* remainder from long division of 1 by 235 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                 /* bring down next decimal digit */
        digit = (seq_t)(r / DEN);/* current digit */
        r = r % DEN;             /* update remainder */
    }
    return digit;
}
