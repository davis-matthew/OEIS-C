/*
 * A042906  Numerators of continued fraction convergents to sqrt(985).
 * https://oeis.org/A042906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=31, a(1)=63; for n≥2 a(n) = c(n)*a(n-1)+a(n-2) where c(n)
 *            repeats 2,1,1,2,62. (c(2)=2, c(3)=1, c(4)=1, c(5)=2, c(6)=62,
 *            c(7)=2, …)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

/* Periodic coefficients of the continued fraction of sqrt(985) after the first term */
static const unsigned int period[] = { 2, 1, 1, 2, 62 };
static const idx_t period_len = 5;

/* Static storage for memoised numerators */
static seq_t *c_numerators = NULL;
static size_t c_cap = 0;          /* allocated capacity */
static idx_t  c_last = (idx_t)-1;/* last computed index; -1 means nothing yet */

/* Ensure that capacity is at least "needed" elements */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **arr)
{
    if (*cap < needed) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort */
            abort();
        }
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* First allocation / initialization if needed */
    if (c_numerators == NULL) {
        ensure_capacity(2, &c_cap, &c_numerators);
        /* initialise first two terms; they are always needed if n>=1 */
        c_numerators[0] = (seq_t)31; /* a(0) */
        c_numerators[1] = (seq_t)63; /* a(1) */
        c_last = 1;
    }

    /* Extend precomputed range if n is beyond current last */
    if (n > c_last) {
        ensure_capacity((size_t)n + 1, &c_cap, &c_numerators);
        /* Compute missing terms from c_last+1 up to n */
        for (idx_t i = c_last + 1; i <= n; ++i) {
            unsigned int coeff;
            if (i == 0) {
                coeff = 31; /* first term */
            } else {
                /* periodic coefficients start at position 1 (i=1) */
                coeff = period[(i - 1) % period_len];
            }
            /* recurrence: a(i) = coeff * a(i-1) + a(i-2) */
            seq_t val = (seq_t)coeff * c_numerators[i - 1] + c_numerators[i - 2];
            c_numerators[i] = val;
        }
        c_last = n;
    }

    return c_numerators[n];
}
