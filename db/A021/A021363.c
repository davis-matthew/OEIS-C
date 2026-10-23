/*
 * A021363  Decimal expansion of 1/359.
 * https://oeis.org/A021363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit (0‑based) of the decimal expansion of
 *            1/359
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the nth decimal digit of 1/359.
   It caches previously computed digits so that any order of calls
   is handled efficiently. The digits are stored as byte values
   (0–9) and converted to seq_t when returned. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached digits: digits[i] holds the i‑th decimal digit (0‑based). */
static uint8_t *digits = NULL;
/* Number of digits currently cached. */
static size_t digits_len = 0;
/* Current remainder in the long division algorithm. */
static uint64_t rem = 1ULL;

/* Return the nth digit of the decimal expansion of 1/359. */
seq_t a(idx_t n)
{
    /* If n is beyond what can be represented in size_t, bail out. */
    if (n > SIZE_MAX - 1)
        return (seq_t)0;

    size_t idx = (size_t)n;

    /* Return cached value if already computed. */
    if (idx < digits_len)
        return (seq_t)digits[idx];

    /* Need to extend the cache up to idx. */
    size_t new_len = idx + 1;
    uint8_t *tmp = (uint8_t *)realloc(digits, new_len * sizeof(uint8_t));
    if (!tmp) {
        /* allocation failed – return 0 as a graceful fallback */
        return (seq_t)0;
    }
    digits = tmp;

    /* Compute the missing digits using long division. */
    for (size_t i = digits_len; i <= idx; ++i)
    {
        uint64_t d = (rem * 10ULL) / 359ULL;   /* next digit 0‑9 */
        rem = (rem * 10ULL) % 359ULL;           /* update remainder */
        digits[i] = (uint8_t)d;
    }
    digits_len = new_len;

    return (seq_t)digits[idx];
}
