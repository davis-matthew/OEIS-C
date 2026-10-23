/*
 * A021475  Decimal expansion of 1/471.
 * https://oeis.org/A021475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = (10^(n+1) div 471) mod 10, i.e. the n‑th digit after the
 *            decimal point in the expansion of 1/471, with offset 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed digits */
    static seq_t *digits     = NULL;    /* array of digits 0–9 */
    static size_t digits_len = 0;       /* number of digits stored */
    static size_t digits_cap = 0;       /* current allocation size */
    static idx_t  rem  = 1;             /* remainder before generating a digit */

    /* grow the cache until it contains digit n */
    for (idx_t i = digits_len; i <= n; ++i) {
        /* long division step */
        idx_t tmp   = rem * 10;            /* 0 <= tmp < 4710 */
        seq_t digit = (seq_t)(tmp / 471);  /* 0–9 */
        rem = tmp % 471;                   /* 0 ≤ rem < 471 */

        /* append the new digit to the cache */
        if (digits_len == digits_cap) {
            size_t new_cap = digits_cap ? digits_cap * 2 : 16;
            seq_t *p = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!p) { /* allocation failed – return 0 as safe fallback */
                return (seq_t)0;
            }
            digits = p;
            digits_cap = new_cap;
        }
        digits[digits_len++] = digit;
    }

    return digits[n];
}
