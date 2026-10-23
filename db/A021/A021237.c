/*
 * A021237  Decimal expansion of 1/233.
 * https://oeis.org/A021237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/233) mod 10 (the n‑th digit after the
 *            decimal point of 1/233)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t r = 1;       /* current remainder, always < 233 */
    seq_t digit = 0;      /* digit to return */

    idx_t i;
    for (i = 0; i <= n; ++i) {
        r = r * 10;        /* bring down a zero */
        digit = r / 233;   /* next decimal digit 0–9 */
        r = r % 233;       /* new remainder */
    }
    return (seq_t)digit;
}
