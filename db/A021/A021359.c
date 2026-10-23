/*
 * A021359  Decimal expansion of 1/355.
 * https://oeis.org/A021359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor((10^n) / 355) mod 10 (digit at position n after the
 *            decimal point of 1/355)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;   /* stored digits 0..len-1 */
    static idx_t len   = 0;        /* number of digits computed */
    static idx_t cap   = 0;        /* current capacity of the array */
    static seq_t rem   = 1;        /* remainder before producing next digit */

    /* Allocate initial buffer if required */
    if (digits == NULL) {
        cap = 16;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Produce digits up to index n */
    while (len <= n) {
        rem      = rem * 10;
        seq_t digit = rem / 355;
        rem      = rem % 355;

        if (len == cap) {
            cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(digits, cap * sizeof(seq_t));
            digits = tmp;
        }
        digits[len++] = digit;
    }

    return digits[n];
}
