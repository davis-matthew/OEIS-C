/*
 * A021663  Decimal expansion of 1/659.
 * https://oeis.org/A021663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th decimal digit (0‑based) of 1/659
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses simple long division with memoisation of digits. */

#include <stdlib.h>
#include <stdint.h>

/* Memoised array of digits as bytes.  Only values 0‑9 are stored. */
static uint8_t *digits   = NULL;
static size_t   digits_cap = 0;
static size_t   digits_len = 0;   /* number of digits already computed */

/* Current remainder after the last computed digit.  Remainder < 659. */
static uint32_t rem = 1;

/* Ensure that digits[0 .. n] are computed and stored. */
static void ensure_digits(idx_t n)
{
    if (n < (idx_t)digits_len) return;           /* already available */

    size_t needed = (size_t)(n + 1);
    if (needed > digits_cap) {
        size_t new_cap = digits_cap ? digits_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        uint8_t *tmp = (uint8_t *)realloc(digits, new_cap * sizeof(uint8_t));
        /* In a real system we would check for NULL, but we assume success. */
        digits = tmp;
        digits_cap = new_cap;
    }

    /* Compute missing digits one by one, updating the remainder. */
    for (idx_t i = digits_len; i <= n; ++i) {
        uint32_t product = rem * 10U;      /* product < 6590, fits in uint32_t */
        uint8_t d = (uint8_t)(product / 659U);
        digits[(size_t)i] = d;
        rem = (uint32_t)(product % 659U);
    }
    digits_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_digits(n);
    return (seq_t)digits[n];
}
