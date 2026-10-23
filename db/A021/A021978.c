/*
 * A021978  Decimal expansion of 1/974.
 * https://oeis.org/A021978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^{n+1}/974) mod 10 (nth decimal digit of 1/974
 *            after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Use 64‑bit arithmetic for the intermediate remainder;
       it never exceeds 9740 and fits easily. */
    unsigned long long rem = 1;   /* start with the numerator 1 */
    unsigned int digit = 0;

    for (idx_t i = 0; i <= n; ++i) {
        rem    = rem * 10ull;
        digit  = rem / 974ull;   /* current decimal digit */
        rem    = rem % 974ull;   /* remainder for next step   */
    }
    return (seq_t)digit;
}
