/*
 * A021486  Decimal expansion of 1/482.
 * https://oeis.org/A021486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the decimal digits of 1/482.
   a(n) is the nth digit after the decimal point of 1/482. */

/* It is computed by performing a long‑division, memoising the
   already computed digits so that future calls are fast. */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *digits = NULL;   /* array of computed digits */
    static size_t cap = 0;        /* allocated capacity of digits */
    static idx_t len   = 0;       /* number of digits already computed (len == 0 -> none) */
    static seq_t rem   = 1;       /* current remainder after the last computed digit */

    /* base values for the first digit */
    if (len == 0) {
        /* For the first digit: 1*10 / 482 = 0  (digit 0)
           remainder becomes 10. */
        cap = 1;
        digits = (seq_t *)malloc(cap * sizeof(seq_t));
        digits[0] = 0;
        len   = 1;
        rem   = 10 % 482;    /* 10 */
    }

    /* Ensure the digit array can hold up to n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 1024;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed; fall back to minimal array */
            tmp = (seq_t *)malloc((size_t)n + 1 * sizeof(seq_t));
            cap = 0;   /* we are not keeping the array any more */
        } else {
            digits = tmp;
            cap = new_cap;
        }
    }

    /* Compute missing digits up to index n */
    for (idx_t i = len; i <= n; ++i) {
        /* long‑division step: digit = (rem*10)/482, new remainder = (rem*10)%482 */
        seq_t next_digit = (rem * 10u) / 482u;
        rem = (rem * 10u) % 482u;
        if (digits) digits[i] = next_digit;
    }
    if (digits) len = n + 1;

    return digits ? digits[n] : 0;
}
