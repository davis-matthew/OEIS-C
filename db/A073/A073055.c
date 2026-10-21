/*
 * A073055  a(n) = product of first n digits in the decimal expansion of Pi,
 *          ignoring decimal point.
 * https://oeis.org/A073055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = product of the first n digits of Pi (ignoring the decimal
 *            point); for n ≥ 33 the product contains the first zero digit and
 *            is therefore 0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 31, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* First 33 digits of Pi after ignoring the decimal point;
   the 33rd digit (index 32) is 0, causing all subsequent products to be 0. */
static const uint8_t pi_digits[33] = {
    3,1,4,1,5,9,2,6,5,3,5,8,9,7,9,3,2,3,8,4,6,2,6,4,3,3,8,3,2,7,9,5,0
};

static seq_t prefix[34];
static bool initialized = false;

/* Pre‑compute the prefix products up to the 33rd digit.  The product for
   n = 33 is 0 because it includes the zero digit. */
static void init_prefix(void)
{
    prefix[0] = 1;
    for (int i = 1; i <= 33; ++i)
        prefix[i] = prefix[i - 1] * pi_digits[i - 1];
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) init_prefix();

    /* For n ≤ 33 we return the pre‑computed prefix product.
       For n > 33 the product is zero (or exceeds the type size),
       but the problem statement guarantees that subsequent terms are 0. */
    if (n > 33) return 0;

    return prefix[n];
}
