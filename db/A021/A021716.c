/*
 * A021716  Decimal expansion of 1/712.
 * https://oeis.org/A021716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of decimal digits of 1/712 computed by long division. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static idx_t cap = 0;
    static idx_t sz = 0;
    static seq_t rem = 0;

    if (digits == NULL) {
        cap = 64;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        rem = 1;  /* initial remainder for 1 / 712 */
    }

    while (sz <= n) {
        seq_t next_rem = rem * (seq_t)10;
        seq_t digit    = next_rem / (seq_t)712;
        rem            = next_rem % (seq_t)712;

        if (sz >= cap) {
            cap *= 2;
            digits = (seq_t *)realloc(digits, cap * sizeof(seq_t));
        }
        digits[sz++] = digit;
    }

    return digits[n];
}
