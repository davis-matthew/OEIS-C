/*
 * A170405  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
   a(0) = 1, a(1) = 12, a(2) = 132, a(3) = 1452, a(4) = 15972, a(5) = 175692.
   For 1 ≤ n < 44:
       a(n) = 2 + 10 * (a(n-1)+a(n-2)+…+a(0))
   For n ≥ 44:
       a(n) = 10 * (a(n-1)+a(n-2)+…+a(n-43)) - 55 * a(n-44)
   (These formulas produce the OEIS sequence A170405.) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored values a(0)..a(max) */
    static size_t cap   = 0;             /* allocated array size */
    static idx_t  max   = (idx_t)-1;     /* highest index computed so far */
    static seq_t sum43 = 0;              /* sum of the last up to 43 values */

    /* Allocate or grow the cache if necessary. */
    if (cache == NULL) {
        cap = (n < 1) ? 1 : (size_t)n + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if ((idx_t)cap <= n) {
        size_t newcap = cap * 2;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp) cache = tmp;
        cap = newcap;
    }

    /* Initialisation for a(0). */
    if (max == (idx_t)-1) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        max = 0;
        sum43 = cache[0];
    }

    /* Compute missing values up to n. */
    if (n > max) {
        for (idx_t i = max + 1; i <= n; ++i) {
            seq_t p;                     /* coefficient from the numerator */

            if (i == 0)          p = (seq_t)1;
            else if (i == 44)    p = (seq_t)1;
            else if (i <= 43)    p = (seq_t)2;
            else                p = (seq_t)0;

            seq_t val = p + (seq_t)10 * sum43;
            if (i >= 44) {
                val -= (seq_t)55 * cache[i - 44];
            }
            cache[i] = val;

            /* Update the sliding window sum of the last 43 terms. */
            sum43 += val;
            if (i >= 43) {
                sum43 -= cache[i - 43];
            }
        }
        max = n;
    }

    return cache[n];
}
