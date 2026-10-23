/*
 * A021722  Decimal expansion of 1/718.
 * https://oeis.org/A021722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit of 1/718 after decimal point
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;          /* current remainder of the long division */
    seq_t digit = 0;         /* digit to return */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;                     /* bring next digit */
        digit = (seq_t)(r / 718ULL); /* compute decimal digit */
        r %= 718ULL;                 /* update remainder */
    }
    return digit;
}
