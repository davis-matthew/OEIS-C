/*
 * A021777  Decimal expansion of 1/773.
 * https://oeis.org/A021777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the nth digit after the decimal point in the expansion
 *            of 1/773
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;
    seq_t digit = 0;
    /* Perform n+1 divisions to obtain the digit at index n. */
    for (idx_t i = 0; i <= n; ++i) {
        rem = rem * 10ul;
        digit = rem / 773ul;
        rem = rem % 773ul;
    }
    return digit;
}
