/*
 * A021370  Decimal expansion of 1/366.
 * https://oeis.org/A021370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: decimal digits of 1/366 after the decimal point (offset 0) */
#include <stdlib.h>
#include <stdbool.h>

/* Function that returns the nth digit (0‑based) of the decimal expansion of 1/366. */
seq_t a(idx_t n)
{
    static seq_t *digits = NULL;   /* cache of computed digits */
    static size_t digits_cap = 0;  /* allocated capacity */
    static size_t digits_len  = 0;  /* number of digits stored */
    static seq_t remainder   = 1;   /* remainder before computing next digit */
    static bool   initialized = false;

    /* Initialize cache on first call */
    if (!initialized) {
        initialized   = true;
        digits_cap    = 128;                       /* initial capacity */
        digits        = (seq_t*)malloc(digits_cap * sizeof(seq_t));
        if (!digits) return 0;                    /* fallback on allocation failure */
        digits_len    = 0;                         /* no digits computed yet */
        remainder     = 1;                         /* start with numerator 1 */
    }

    /* Compute digits up to n, extending the cache as needed */
    while (digits_len <= (size_t)n) {
        /* Multiply remainder by 10 and extract the next digit */
        seq_t r = (remainder * 10);
        seq_t d = (r / 366);
        remainder = (r % 366);

        /* Grow cache if necessary */
        if (digits_len >= digits_cap) {
            digits_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(digits, digits_cap * sizeof(seq_t));
            if (!tmp) {  /* allocation failed – return 0 for safety */
                free(digits);
                digits = NULL;
                digits_cap = digits_len = 0;
                initialized = false;
                return 0;
            }
            digits = tmp;
        }

        digits[digits_len++] = d;   /* store the computed digit */
    }

    return digits[(size_t)n];
}
