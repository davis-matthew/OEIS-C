/*
 * A021977  Decimal expansion of 1/973.
 * https://oeis.org/A021977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/973) mod 10 (the n-th digit of the decimal
 *            expansion of 1/973, offset 0)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of previously computed digits */
static seq_t *digits = NULL;          /* array of computed digits */
static size_t digits_cap = 0;         /* capacity of the array */
static idx_t digits_len = 0;          /* number of digits stored */
static idx_t remainder = 1;           /* current remainder for digit generation */
static bool initialized = false;     /* one‑time init flag */

/* Ensure the digit cache covers indices up to n */
static void ensure_cache(idx_t n) {
    if (!initialized) {
        initialized = true;
        digits_cap = 256;            /* initial capacity */
        digits_len = 0;
        digits = (seq_t*)malloc(digits_cap * sizeof(seq_t));
    }
    while (digits_len <= n) {
        if (digits_len + 1 >= digits_cap) {
            digits_cap *= 2;
            digits = (seq_t*)realloc(digits, digits_cap * sizeof(seq_t));
        }
        /* Generate next digit */
        remainder *= 10ULL;
        seq_t digit = (seq_t)(remainder / 973ULL);
        remainder %= 973ULL;
        digits[digits_len++] = digit;
    }
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return digits[n];
}
