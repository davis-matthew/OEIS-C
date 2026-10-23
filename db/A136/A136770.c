/*
 * A136770  n! never ends in this many 0's in base 9.
 * https://oeis.org/A136770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A136770:
 * a(n) is the n-th positive integer that never occurs as floor(v3(m!)/2)
 * (the number of trailing zeros of m! in base 9).  The implementation
 * generates the missing values by iteratively increasing n, adding the
 * exponent of 3 in n to a running total of v3(n!), computing
 * t = ⌊v3/2⌋, and recording every integer between successive t values
 * that is skipped.  This yields the requested a(n). */
#include <stdlib.h>

seq_t
a(idx_t n)
{
    /* static state for incremental generation */
    static idx_t   last_n       = 1;   /* last n that was processed   */
    static idx_t   v3           = 0;   /* v3(last_n!)                 */
    static idx_t   t_prev       = 0;   /* floor(v3/2) at last_n        */
    static seq_t  *missing      = NULL;/* array of missing numbers     */
    static size_t  missing_len  = 0;   /* current number of missing    */
    static size_t  missing_cap  = 0;   /* current capacity of array    */

    /* allocate space for the first missing number if needed */
    if (missing_len < 1 && missing_cap == 0) {
        missing_cap = 8;
        missing = (seq_t *)realloc(missing, missing_cap * sizeof(seq_t));
        if (!missing)
            abort();              /* allocation failure */
    }

    /* generate missing numbers until we have at least n of them */
    while (missing_len < n) {
        /* advance to the next n and add its 3‑adic exponent to v3 */
        ++last_n;
        idx_t m = last_n;
        int   e = 0;
        while ((m % 3) == 0) {
            ++e;
            m /= 3;
        }
        v3 += e;

        /* compute current t and the jump from the previous t */
        idx_t t = v3 >> 1;           /* floor(v3/2) */
        idx_t diff = t - t_prev;

        if (diff > 1) {
            /* every integer between t_prev+1 and t-1 is missing */
            for (idx_t k = t_prev + 1; k < t; ++k) {
                if (missing_len == missing_cap) {
                    size_t new_cap = missing_cap * 2;
                    if (new_cap == 0)
                        new_cap = 8;
                    seq_t *tmp =
                        (seq_t *)realloc(missing,
                                         new_cap * sizeof(seq_t));
                    if (!tmp)
                        abort();
                    missing = tmp;
                    missing_cap = new_cap;
                }
                missing[missing_len++] = (seq_t)k;
            }
        }
        t_prev = t;
    }

    return missing[n - 1];
}
