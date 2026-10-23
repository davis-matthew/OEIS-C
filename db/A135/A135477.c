/*
 * A135477  Positive numbers of the form x^2-6^y with repetition.
 * https://oeis.org/A135477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   big_pow, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes A135477: Positive numbers of the form x^2-6^y,
   with repetition, sorted in non‑decreasing order.
   The function collects all such values up to a growing limit,
   sorts them once, and returns the n‑th element (1‑based).
   Duplicates from different (x,y) pairs are preserved.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

/* Comparator for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Persistent array of collected values */
    static seq_t *values = NULL;
    static size_t values_len = 0;
    static size_t values_cap = 0;

    /* If we already have enough values, return the requested one */
    if (n <= values_len)
        return values[n - 1];

    /* --------------------------------------------------------------------
       1. Generate values up to an initial LIMIT.
       2. If not enough, double LIMIT and regenerate.
       -------------------------------------------------------------------- */
    seq_t LIMIT = (seq_t)1000000;   /* start with 1,000,000 */
    while (true)
    {
        /* Free old array if it exists */
        if (values != NULL) {
            free(values);
            values = NULL;
        }

        /* Allocate initial capacity and reset counter */
        values_cap = 1024;
        values = (seq_t *)malloc(values_cap * sizeof(seq_t));
        if (!values) abort();
        values_len = 0;

        /* Enumerate all pairs (y,x) such that x²–6^y ≤ LIMIT and is positive */
        for (idx_t y = 0;; ++y)
        {
            seq_t pw = big_pow((seq_t)6, (seq_t)y);     /* 6^y        */
            seq_t sqrt_pw = floor_sqrt(pw);            /* ⌊√6^y⌋      */
            seq_t x0 = sqrt_pw + 1;                    /* minimal x   */
            seq_t val_min = (x0 * x0) - pw;             /* minimal val */

            if (val_min > LIMIT)                       /* too large   */
                break;

            for (seq_t x = x0; ; ++x)
            {
                seq_t val = (x * x) - pw;
                if (val > LIMIT)
                    break;

                /* Grow array if necessary */
                if (values_len == values_cap)
                {
                    values_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(values, values_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    values = tmp;
                }
                values[values_len++] = val;
            }
        }

        /* Sort the collected values */
        qsort(values, values_len, sizeof(seq_t), cmp_seq_t);

        /* If we now have enough values stop */
        if (n <= values_len)
            break;

        /* Not enough – double the limit and try again */
        LIMIT *= 2;
    }

    /* Return the requested element (1‑based) */
    return values[n - 1];
}
