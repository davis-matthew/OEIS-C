/*
 * A021282  Decimal expansion of 1/278.
 * https://oeis.org/A021282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/278) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* long‑division method to extract the n-th decimal digit of 1/278 */
    unsigned long long rem = 1ULL;   /* current remainder */
    unsigned long long digit = 0ULL; /* digit at the current step */

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10ULL;                 /* bring down next decimal place */
        digit = rem / 278ULL;         /* the digit for this position */
        rem %= 278ULL;                /* new remainder for next step   */
    }
    return (seq_t)digit;
}
