/*
 * A021642  Decimal expansion of 1/638.
 * https://oeis.org/A021642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/638 after the decimal point */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed digits */
    static seq_t *digits = NULL;
    static size_t len = 0;       /* number of digits currently cached */
    static uint64_t rem = 1;     /* remainder before the next digit  */

    /* Compute additional digits if necessary */
    if ((idx_t)len <= n) {
        size_t needed = n + 1;
        if (digits == NULL) {                 /* initial allocation */
            digits = (seq_t *)malloc(needed * sizeof(seq_t));
        } else if (len < needed) {            /* grow the cache */
            seq_t *tmp = (seq_t *)realloc(digits, needed * sizeof(seq_t));
            if (tmp == NULL) {                /* allocation failure (should not happen here) */
                return 0;
            }
            digits = tmp;
        }
        /* Long‑division to produce missing digits */
        for (size_t i = len; i <= (size_t)n; ++i) {
            rem = rem * 10;
            digits[i] = rem / 638;   /* floor division gives the next decimal digit */
            rem %= 638;             /* remainder for next step */
            ++len;
        }
    }
    return digits[(size_t)n];
}
