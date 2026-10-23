/*
 * A022001  Decimal expansion of 1/997.
 * https://oeis.org/A022001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) / 997⌋ mod 10 The nth digit after the decimal
 *            point of 1/997 is obtained by iterative long division: a(n) =
 *            (r*10)/997, r = (r*10) % 997, starting with r = 1
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;    /* remainder of the long division */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        digit = (seq_t)((r * 10) / 997);
        r = (r * 10) % 997;
    }
    return digit;
}
