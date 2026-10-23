/*
 * A021089  Decimal expansion of 1/85.
 * https://oeis.org/A021089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth decimal digit after the decimal point of 1/85
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


seq_t a(idx_t n)
{
    /* static cache of computed digits and current remainder */
    static seq_t *digits = NULL;
    static size_t digits_len = 0;
    static seq_t rem = 1;   /* remainder before first digit (initially 1) */

    /* If already computed up to n, return the stored value */
    if (n < (idx_t)digits_len)
        return digits[n];

    /* Resize array to hold digits up to index n */
    size_t new_len = (size_t)n + 1;
    if (digits == NULL)
        digits = (seq_t *)malloc(new_len * sizeof(seq_t));
    else if (new_len > digits_len) {
        seq_t *tmp = (seq_t *)realloc(digits, new_len * sizeof(seq_t));
        if (tmp != NULL)
            digits = tmp;
    }

    /* Compute remaining digits */
    for (size_t i = digits_len; i <= (size_t)n; ++i) {
        uint64_t t = (uint64_t)rem * 10;
        seq_t digit = (seq_t)(t / 85);
        rem = (seq_t)(t % 85);
        digits[i] = digit;
    }

    digits_len = new_len;
    return digits[n];
}
