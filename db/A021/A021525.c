/*
 * A021525  Decimal expansion of 1/521.
 * https://oeis.org/A021525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit after the decimal point in the expansion of
 *            1/521
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t digits[521];
static idx_t period = 0;

seq_t a(idx_t n)
{
    if (period == 0) {
        uint64_t r = 1;          /* initial remainder for 1/521 */
        idx_t i = 0;
        do {
            r *= 10;
            seq_t d = r / 521;   /* current digit */
            digits[i++] = d;
            r %= 521;            /* new remainder */
        } while (r != 1 && i < 521);   /* period ends when remainder returns to 1 */
        period = i;
    }
    return digits[n % period];
}
