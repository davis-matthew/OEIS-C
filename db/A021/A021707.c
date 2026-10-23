/*
 * A021707  Decimal expansion of 1/703.
 * https://oeis.org/A021707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th decimal digit of 1/703 (the sequence is the
 *            decimal expansion of 1/703)
 * Range:     u8 none, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t digits[64];   /* large enough for the period (18) */
static size_t period = 0;

seq_t a(idx_t n)
{
    /* Lazy initialization: compute the repeating decimal digits of 1/703 once. */
    if (period == 0) {
        seq_t remainder = 1;      /* start with 1/703 */
        size_t i = 0;
        do {
            seq_t d = (remainder * 10) / 703;
            digits[i++] = d;
            remainder = (remainder * 10) % 703;
        } while (remainder != 1);
        period = i;              /* period length is 18 for 1/703 */
    }
    /* Return the digit corresponding to position n (0‑based). */
    return digits[n % period];
}
