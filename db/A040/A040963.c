/*
 * A040963  Continued fraction for sqrt(995).
 * https://oeis.org/A040963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(995).
   The sequence is  a(0) = floor(sqrt(995)) = 31,
   and for n>=1 it repeats the periodic part of the continued fraction
   of sqrt(995), namely
        1, 1, 5, 4, 3, 12, 3, 4, 5, 1, 1, 62
   (period length 12). */
/* This implementation builds the period once, stores it in a static
   array, and returns terms on demand. */
#include <stddef.h>
#include <stdlib.h>       /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* Special case: the first term */
    if (n == 0)
        return (seq_t)31;          /* floor(sqrt(995)) */

    /* static storage for the periodic part */
    static seq_t *period = NULL;   /* array of coefficients */
    static size_t period_len = 0;  /* length of the period */

    /* Build the period on first call */
    if (period == NULL) {
        const seq_t D   = (seq_t)995;
        const seq_t a0  = (seq_t)31;   /* floor sqrt(D) */
        seq_t m = 0;
        seq_t d = 1;
        seq_t a = a0;                 /* current coefficient */

        /* Initially guess a capacity; will grow if necessary */
        size_t cap   = 16;
        period = (seq_t *)malloc(cap * sizeof(seq_t));
        size_t len  = 0;

        /* Generate terms until the last term 2*a0 is reached */
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;          /* next coefficient */

            /* Store the coefficient */
            if (len == cap) {
                cap <<= 1;
                period = (seq_t *)realloc(period, cap * sizeof(seq_t));
            }
            period[len++] = a;
        } while (a != 2 * a0);         /* last term of the period */

        period_len = len;
    }

    /* Return the required term: cycle through the period */
    size_t idx = (size_t)(n - 1) % period_len;
    return period[idx];
}
