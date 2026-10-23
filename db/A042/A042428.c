/*
 * A042428  Numerators of continued fraction convergents to sqrt(742).
 * https://oeis.org/A042428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(742).
   They satisfy the recurrence
     h(-2)=0, h(-1)=1,
     h(n) = a(n)*h(n-1) + h(n-2)  for n >= 0,
   where a(0)=floor(sqrt(742))=27 and a(n) for n>=1
   are the coefficients of the continued fraction expansion of sqrt(742). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of convergents */
    static seq_t *conv = NULL;
    static size_t conv_cap = 0;
    static size_t conv_len  = 0;          /* number of stored convergents */

    /* Cache of the periodic part of the continued fraction of sqrt(742) */
    static seq_t *period = NULL;
    static size_t period_cap = 0;
    static size_t period_len = 0;          /* number of coefficients in the period */

    /* basic data */
    const seq_t a0   = (seq_t)27;                 /* floor(sqrt(742))          */
    const seq_t D    = (seq_t)742;                /* the squarefree part       */
    const seq_t stop = 2 * a0;                    /* last coefficient in a period */

    /* generate the period on first call */
    if (period_len == 0) {
        period_cap = 16;
        period = (seq_t *)malloc(period_cap * sizeof(seq_t));
        if (!period) return (seq_t)0;              /* allocation failure */

        seq_t m = 0, d = 1, a = a0;
        do {
            m = d * a - m;
            d = (D - m * m) / d;
            a = (a0 + m) / d;

            if (period_len == period_cap) {
                period_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(period, period_cap * sizeof(seq_t));
                if (!tmp) { free(period); return (seq_t)0; }
                period = tmp;
            }
            period[period_len++] = a;
        } while (a != stop);
    }

    /* Ensure the convergents cache can hold n */
    size_t tn = (size_t)n;
    if (tn + 1 > conv_cap) {
        size_t new_cap = conv_cap ? conv_cap * 2 : 16;
        if (new_cap < tn + 1) new_cap = tn + 1;
        seq_t *tmp = (seq_t *)realloc(conv, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        conv = tmp;
        conv_cap = new_cap;
    }

    /* Compute convergents up to index n */
    if (tn + 1 > conv_len) {
        /* initialise first element if not yet done */
        if (conv_len == 0)
            conv[0] = a0;
        /* compute missing elements */
        for (size_t i = conv_len; i <= tn; ++i) {
            if (i == 0) {
                /* already set above */
            } else if (i == 1) {
                conv[1] = period[(i - 1) % period_len] * conv[0] + (seq_t)1;
            } else {
                conv[i] = period[(i - 1) % period_len] * conv[i - 1] + conv[i - 2];
            }
        }
        conv_len = tn + 1;
    }

    return conv[tn];
}
