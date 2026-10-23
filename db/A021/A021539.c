/*
 * A021539  Decimal expansion of 1/535.
 * https://oeis.org/A021539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit after the decimal point of 1/535 (a(0)=0,
 *            a(1)=0, a(2)=1, ...)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Remainder after the integer part (1/535 < 1). */
    seq_t rem = 1;           /* 1 * 10^0 */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= (seq_t)10;               /* shift decimal point one place */
        seq_t digit = rem / (seq_t)535; /* integer part of the division */
        rem = rem % (seq_t)535;         /* new remainder */
        if (i == n)                     /* returned digit is the required one */
            return digit;
    }
    /* Control will never reach here, but keep the compiler quiet. */
    return (seq_t)0;
}
