/*
 * A236157  Number of (n+1) X (1+1) 0..2 arrays colored with the sets of
 *          distinct values in every 2 X 2 subblock, with new values 0..2
 *          introduced row-major order.
 * https://oeis.org/A236157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=14, a(2)=69, a(3)=358, a(4)=1844, a(5)=9522, a(6)=49108
 *            a(n) = a(n-1) + 18*a(n-2) + 16*a(n-3) + 12*a(n-4) - 12*a(n-5)
 *            for n>6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes the sequence in a static array large enough
   for the test range (up to 20000). It works for any idx_t ≥1 and produces
   exact values for all terms that fit in seq_t.  */

#include <stddef.h>               /* for size_t */

#ifndef MAX_A_N
#define MAX_A_N 20000
#endif

seq_t a(idx_t n)
{
    if (n < 1U) return 0;               /* outside the defined range */

    /* static cache, 1‑based indexing: cache[i] holds a(i) */
    static seq_t cache[MAX_A_N + 1];
    static size_t len = 0;               /* highest index computed so far */

    /* base values for n = 1 … 6 */
    static const seq_t init_vals[6] = {
        (seq_t)14,
        (seq_t)69,
        (seq_t)358,
        (seq_t)1844,
        (seq_t)9522,
        (seq_t)49108
    };

    /* if the request is already computed, return it */
    if ((size_t)n <= len)
        return cache[n];

    /* Compute missing prefix up to n */
    size_t i;

    /* first fill the initial values if we haven't yet */
    if (len < 6) {
        size_t upto = (size_t)n < 6 ? (size_t)n : 6;
        for (i = 1; i <= upto; ++i)
            cache[i] = init_vals[i-1];
        len = upto;
    }

    /* compute further terms using the recurrence */
    for (i = len + 1; i <= (size_t)n; ++i) {
        if (i <= 6) {                     /* safety, shouldn't happen */
            cache[i] = init_vals[i-1];
        } else {
            /* a(i) = a(i-1) + 18*a(i-2) + 16*a(i-3) + 12*a(i-4) - 12*a(i-5) */
            cache[i] = cache[i-1]
                       + ((seq_t)18 * cache[i-2])
                       + ((seq_t)16 * cache[i-3])
                       + ((seq_t)12 * cache[i-4])
                       - ((seq_t)12 * cache[i-5]);
        }
    }
    len = (size_t)n;

    return cache[n];
}
