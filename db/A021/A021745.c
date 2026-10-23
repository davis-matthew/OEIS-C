/*
 * A021745  Decimal expansion of 1/741.
 * https://oeis.org/A021745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A021745: decimal expansion of 1/741.
   Formula:  a(n) = ((10^(n+1)) / 741) mod 10  (digit at position n after the decimal point)
*/

#include <stdbool.h>

/* static cache for the 18‑digit repeating pattern */
static seq_t digits[18];
static bool initialized = false;

/* initialise the repeating pattern via long division */
static void init_digits(void)
{
    int rem = 1;                     /* remainder before first digit (numerator) */
    for (int i = 0; i < 18; ++i) {
        rem *= 10;
        digits[i] = (seq_t)(rem / 741);   /* integer part gives the digit */
        rem = rem % 741;                   /* new remainder */
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_digits();
    }
    /* period length is 18 */
    return digits[(int)(n % 18u)];
}
