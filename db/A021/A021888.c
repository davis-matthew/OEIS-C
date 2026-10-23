/*
 * A021888  Decimal expansion of 1/884.
 * https://oeis.org/A021888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Decimal digit of 1/884 at position n (offset 0) */
#include <stdint.h>
seq_t a(idx_t n)
{
    uint64_t remainder = 1;    /* 1 % 884 = 1 */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10;                     /* bring next digit */
        digit = (seq_t)(remainder / 884);    /* digit at this position */
        remainder %= 884;                    /* new remainder */
    }
    return digit;
}
