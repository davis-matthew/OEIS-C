/*
 * A021127  Decimal expansion of 1/123.
 * https://oeis.org/A021127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit of the decimal expansion of 1/123 at position n
 *            (0-indexed)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;          /* remainder after division of 1 by 123 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10ULL;          /* bring next digit to the front */
        digit = (seq_t)(r / 123ULL);
        r %= 123ULL;         /* remainder for next step */
    }
    return digit;
}
