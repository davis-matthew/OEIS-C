/*
 * A021435  Decimal expansion of 1/431.
 * https://oeis.org/A021435
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

/* Sequence: a(n) = the n‑th decimal digit of 1/431 after the decimal point
   (offset 0, so a(0) is the first digit following the point):
   a(n) = ⌊(10·r_n)/431⌋,
   where r_n is the remainder before producing digit n (r_0 = 1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;          /* digits[0] … digits[digits_len‑1] */
    static size_t digits_len = 0;          /* number of digits stored */
    static idx_t rem = 1;                  /* remainder after last digit */
    const idx_t den = 431;                 /* the denominator */

    /* Grow the array until it contains at least n+1 digits. */
    if (digits_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(digits, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failure: return 0 as a safe fallback */
            return (seq_t)0;
        }
        digits = tmp;
        /* Compute missing digits one by one. */
        while (digits_len < new_len) {
            /* Standard long‑division step. */
            idx_t d = (rem * 10) / den;
            rem = (rem * 10) % den;
            digits[digits_len] = (seq_t)d;
            ++digits_len;
        }
    }
    return digits[(size_t)n];
}
