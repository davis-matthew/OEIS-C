/*
 * A021295  Decimal expansion of 1/291.
 * https://oeis.org/A021295
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

/* Sequence A021295: a(n) is the n-th decimal digit after the point of 1/291,
   which is periodic with period 96. This implementation computes the value
   by performing long division once and then indexing into the repeat block. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t digits[96];
    static bool initialized = false;
    if (!initialized) {
        idx_t r = 1;                    /* initial remainder of 1 / 291 */
        for (idx_t i = 0; i < 96; ++i) {
            idx_t temp = r * 10;        /* multiply remainder by 10 */
            digits[i]  = temp / 291;    /* quotient is the next digit   */
            r          = temp % 291;    /* new remainder */
        }
        initialized = true;
    }
    return digits[n % 96];
}
