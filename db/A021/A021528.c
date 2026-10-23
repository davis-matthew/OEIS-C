/*
 * A021528  Decimal expansion of 1/524.
 * https://oeis.org/A021528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/524) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;          /* current remainder of the division 1 / 524 */
    uint64_t digit = 0;        /* digit at current position */

    /* Perform n+1 steps of long division to generate the digit at index n */
    for (uint64_t i = 0; i <= n; ++i) {
        rem *= 10;                 /* bring down next decimal place */
        digit = rem / 524;         /* digit = floor(rem / 524) */
        rem -= digit * 524;        /* remainder = rem % 524 */
    }

    return (seq_t)digit;
}
