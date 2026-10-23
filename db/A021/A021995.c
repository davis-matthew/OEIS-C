/*
 * A021995  Decimal expansion of 1/991.
 * https://oeis.org/A021995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = ((10^(n+1)) div 991) mod 10 (the nth decimal digit of
 *            1/991, offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;     /* remainder of 1/991 * 10^k */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;            /* shift left one decimal place   */
        digit = rem / 991;    /* next decimal digit              */
        rem   = rem % 991;    /* new remainder for next step     */
    }
    return digit;
}
