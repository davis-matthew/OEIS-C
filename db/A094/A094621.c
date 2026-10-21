/*
 * A094621  Expansion of x*(11+13*x+20*x^2) / ( (x-1)*(1+x)*(10*x^2-1) ).
 * https://oeis.org/A094621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A094621
   Rational generating function:  x*(11+13*x+20*x^2) / ((x-1)*(1+x)*(10*x^2-1))
   Recurrence (derived from the denominator):
       a(0) = 0
       a(1) = 11
       a(2) = 13
       a(3) = 141
       a(n) = 11*a(n-2) - 10*a(n-4)   for n ≥ 4
   This recurrence is used to compute all terms.  The function caches
   previously computed values so multiple random-access calls are efficient.
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values are stored in a static array.
       The array grows dynamically to accommodate the largest index
       requested so far.  This guarantees O(1) amortized access
       after the first request.  */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;   /* number of pre-computed terms */

    if (n >= buf_len) {
        /* Grow the buffer to hold indices 0 .. n */
        idx_t newlen = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, newlen * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort (no I/O allowed) */
            abort();
        }
        buf = tmp;

        /* Compute terms from the previous length up to n */
        for (idx_t i = buf_len; i <= n; ++i) {
            seq_t val;
            if (i == 0)
                val = 0;
            else if (i == 1)
                val = 11;
            else if (i == 2)
                val = 13;
            else if (i == 3)
                val = 141;
            else
                /* a(i) = 11*a(i-2) - 10*a(i-4) */
                val = 11 * buf[i - 2] - 10 * buf[i - 4];
            buf[i] = val;
        }
        buf_len = newlen;
    }
    return buf[n];
}
