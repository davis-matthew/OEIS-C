/*
 * A021797  Decimal expansion of 1/793.
 * https://oeis.org/A021797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = nth digit of the decimal expansion of 1/793
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A021797)                                              */
#include <stddef.h>
#include <stdlib.h>

/* static state for memoization */
static seq_t *digits = NULL;      /* array of computed digits          */
static size_t digits_size = 0;    /* current allocated size            */
static idx_t   cached_n = 0;      /* highest index that has been computed */
static seq_t   rem = 1;           /* remainder before computing the next digit */

/* Return the nth digit of 1/793 after the decimal point. */
seq_t a(idx_t n)
{
    /* Initialize cache and compute the first digit if needed. */
    if (digits == NULL) {
        digits_size = 1;
        digits = (seq_t*)malloc(digits_size * sizeof(seq_t));
        if (digits == NULL) return 0;            /* allocation failed */
        seq_t r = rem * (seq_t)10;               /* r = 1*10           */
        seq_t d = r / (seq_t)793;                /* d = 0              */
        rem = r % (seq_t)793;                    /* rem = 10           */
        digits[0] = d;
        cached_n = 0;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if ((size_t)n >= digits_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(digits, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;              /* allocation failed */
        digits = tmp;
        digits_size = new_size;
    }

    /* Compute any missing digits up to index n. */
    if (n > cached_n) {
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t r = rem * (seq_t)10;
            seq_t d = r / (seq_t)793;
            rem = r % (seq_t)793;
            digits[i] = d;
        }
        cached_n = n;
    }

    return digits[(size_t)n];
}
