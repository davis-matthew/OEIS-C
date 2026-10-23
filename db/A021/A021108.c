/*
 * A021108  Decimal expansion of 1/104.
 * https://oeis.org/A021108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * ((10^n) mod 104) / 104)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/104, periodic with period 6 after n = 3 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle offset 0,1,2 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)9;

    /* periodic part after the 9th digit */
    static const seq_t period[6] = {6, 1, 5, 3, 8, 4};
    idx_t idx = (n - 3) % 6;
    return period[idx];
}
