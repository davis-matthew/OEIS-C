/*
 * A094025  Expansion of (1+3x)/((1-x^2)(1-3x^2)).
 * https://oeis.org/A094025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(n)=3*a(n-1) if n odd; a(n)=a(n-1)+1 if n even */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *vals   = NULL;
    static idx_t  last_n = 0;
    static idx_t  cap    = 0;

    if (vals == NULL) {
        cap   = 128;
        vals  = (seq_t*)malloc(cap * sizeof(seq_t));
        vals[0] = (seq_t)1;
    }

    /* Grow the table if needed */
    if (n > last_n) {
        if (n >= cap) {
            idx_t newcap = cap;
            while (newcap <= n) newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(vals, newcap * sizeof(seq_t));
            vals = tmp;
            cap  = newcap;
        }
        for (idx_t k = last_n + 1; k <= n; ++k) {
            if (k & 1)                     /* odd index */
                vals[k] = vals[k-1] * (seq_t)3;
            else                            /* even index */
                vals[k] = vals[k-1] + (seq_t)1;
        }
        last_n = n;
    }

    return vals[n];
}
