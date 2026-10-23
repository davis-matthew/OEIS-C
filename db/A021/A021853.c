/*
 * A021853  Decimal expansion of 1/849.
 * https://oeis.org/A021853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1) / 849) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the n-th decimal digit of 1/849, with a(0) being the first digit after the decimal point. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static storage for digits already computed */
    static seq_t *digits = NULL;
    static size_t digits_len = 0;
    static size_t digits_cap = 0;
    /* Remainder used in the long‑division procedure */
    static uint64_t remainder = 1;   /* 1 / 849 initially */

    /* Return a previously computed digit if available */
    if (n < digits_len)
        return digits[n];

    /* Ensure the array can hold up to digit n */
    size_t needed = n + 1;
    if (needed > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap << 1 : 128;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_arr = (seq_t*)realloc(digits, new_cap * sizeof(seq_t));
        digits = new_arr;
        digits_cap = new_cap;
    }

    /* Generate digits until we reach the required index */
    while (digits_len < needed) {
        remainder *= 10ull;
        seq_t digit = (seq_t)(remainder / 849ull);
        remainder = remainder % 849ull;
        digits[digits_len++] = digit;
    }

    return digits[n];
}
