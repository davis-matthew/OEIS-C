/*
 * A021218  Decimal expansion of 1/214.
 * https://oeis.org/A021218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit at index n of the decimal expansion of 1/214 *
 *            (0-based, a(0) = first digit after the decimal point)
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: cached digits, current remainder, and array capacity */
    static seq_t *digits = NULL;
    static size_t  cap      = 0;
    static size_t  len      = 0;
    static seq_t   rem      = 1; /* remainder before computing the next digit */

    /* Initialise the cache once */
    if (!digits) {
        cap = 128;
        digits = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!digits) return 0; /* fallback if allocation fails */
    }

    /* Ensure we have computed digits up to index n */
    if (n >= len) {
        /* Grow storage if necessary */
        while (cap <= (size_t)n) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t*)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) break;           /* allocation failed – keep old size */
            digits = tmp;
            cap = new_cap;
        }

        /* Compute missing digits */
        for (size_t i = len; i <= (size_t)n; ++i) {
            rem = rem * (seq_t)10;            /* multiply remainder by 10 */
            digits[i] = rem / (seq_t)214;     /* next digit */
            rem = rem % (seq_t)214;           /* update remainder */
        }
        len = (size_t)n + 1;
    }

    return digits[(size_t)n];
}
