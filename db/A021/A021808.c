/*
 * A021808  Decimal expansion of 1/804.
 * https://oeis.org/A021808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/804) mod 10 (decimal digits of 1/804)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t r = 1;               /* current remainder */
    seq_t digit = 0;           /* last computed digit */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10;               /* shift remainder left by one decimal place */
        digit = r / 804;      /* digit at this position */
        r %= 804;              /* new remainder for next digit */
    }
    return digit;
}
