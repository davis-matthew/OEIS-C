/*
 * A059629  Carryless multiplication 6 X n base 10.
 * https://oeis.org/A059629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = carryless multiplication of 6 and n in base 10 * (each
 *            decimal digit d of n is replaced by (6*d) mod 10, * and the
 *            place values are preserved; no carries are used)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;   /* accumulator for the product */
    seq_t mult   = 1;   /* place value (10^k) for the current digit */

    /* Work with a copy of n so that the original argument is unchanged. */
    idx_t temp = n;

    /* Process each decimal digit from least significant to most. */
    while (temp > 0) {
        unsigned digit   = temp % 10;           /* digit of n (0..9) */
        unsigned prod    = (digit * 6) % 10;    /* carryless product digit */
        result += prod * mult;                  /* place the new digit */
        mult   *= 10;                           /* shift to next higher place */
        temp   /= 10;                           /* move to next digit */
    }

    return result;
}
