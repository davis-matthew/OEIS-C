/*
 * A021125  Decimal expansion of 1/121.
 * https://oeis.org/A021125
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/121) mod 10 (nth decimal digit of 1/121)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Remainder of 1 / 121 during long division, in the range 0..120 */
    uint64_t rem = 1;
    seq_t digit = 0;

    /* Perform long division n+1 times to reach the nth decimal digit
       (index 0 is the first digit after the decimal point). */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;                 /* bring down next digit */
        digit = (seq_t)(rem / 121ULL);/* next decimal digit */
        rem %= 121ULL;                /* new remainder */
    }

    return digit;
}
