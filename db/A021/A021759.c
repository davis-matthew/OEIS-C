/*
 * A021759  Decimal expansion of 1/755.
 * https://oeis.org/A021759
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
 * Formula: a(n) = (floor(10^{n+1} / 755) % 10)
 *    The sequence consists of the decimal digits of 1/755.
 *    The nth term (offset 0) is obtained by long division: multiply the
 *    current remainder by 10, take the quotient by 755 as the digit, and
 *    keep the new remainder for the next step.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache of already computed digits */
    static seq_t *digits = NULL;
    static size_t cap = 0;
    static size_t len = 0;          /* number of digits stored */
    static idx_t rem = 1;           /* remainder before next digit (initial: 1) */

    /* extend the cache if needed */
    if (len <= n) {
        /* grow capacity if necessary */
        if (cap == 0) {
            cap = 4;
            digits = (seq_t*)malloc(cap * sizeof(seq_t));
        }
        while (len <= n) {
            if (len >= cap) {
                cap <<= 1;
                digits = (seq_t*)realloc(digits, cap * sizeof(seq_t));
            }
            /* long‑division step */
            rem *= 10;                       /* shift left one decimal place */
            seq_t digit = (seq_t)(rem / 755); /* digit = floor(rem / 755) */
            rem = rem % 755;                 /* new remainder */
            digits[len++] = digit;           /* store digit */
        }
    }
    return digits[n];
}
