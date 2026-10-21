/*
 * A071843  Gives an LCD representation of n.
 * https://oeis.org/A071843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071843 – LCD representation of a decimal number.
    For n with decimal digits d_k … d_1 d_0 (k ≥ 0),
    a(n) = a(d_k … d_1) + 128 * digit[d_0],
    where digit[x] is the 7‑segment mask of decimal digit x.
    Base case:  a(d) = digit[d] for 0 ≤ d ≤ 9.               */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 7‑segment encodings for digits 0–9 (decimal) */
    static const seq_t digit[10] = {
        (seq_t)119, /* 0 */
        (seq_t)17,  /* 1 */
        (seq_t)107, /* 2 */
        (seq_t)59,  /* 3 */
        (seq_t)29,  /* 4 */
        (seq_t)62,  /* 5 */
        (seq_t)126, /* 6 */
        (seq_t)19,  /* 7 */
        (seq_t)127, /* 8 */
        (seq_t)63   /* 9 */
    };

    /* base case: single decimal digit */
    if (n < 10)
        return digit[(size_t)n];

    /* recursive step: keep the higher‑order digits, add 128×mask of the
       current least‑significant digit.  Multiplication by 128 is a safe
       left‑shift on all supported integer widths. */
    return a(n / 10) + ((seq_t)128) * digit[(size_t)(n % 10)];
}
