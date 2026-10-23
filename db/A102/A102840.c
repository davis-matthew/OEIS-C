/*
 * A102840  a(0)=0, a(1)=1, a(n)=((2*n-1)*a(n-1)-5*n*a(n-2))/(n-1).
 * https://oeis.org/A102840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1, a(n) = ((2*n-1)*a(n-1) - 5*n*a(n-2)) / (n-1)
 *            for n >= 2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 23, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *seq   = NULL;
    static idx_t cap    = 0;   /* allocated capacity */
    static idx_t last   = 0;   /* highest index already computed */

    /* Ensure capacity for at least n+1 elements, but at least 2 for the
       initial values a(0) and a(1). */
    idx_t needed = n + 1;
    if (needed < 2) needed = 2;
    if (needed > cap) {
        seq_t *tmp = (seq_t *)realloc(seq, needed * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure */
        seq = tmp;
        cap = needed;
    }

    /* Initialise the first two terms if they are not yet stored. */
    if (last < 1) {
        seq[0] = (seq_t)0;
        seq[1] = (seq_t)1;
        last   = 1;
    }

    /* Compute all missing terms up to index n. */
    for (idx_t k = last + 1; k <= n; ++k) {
        /* Products are evaluated in seq_t to avoid overflow in
           intermediate integer types. */
        seq_t factor1 = (seq_t)((2 * k) - 1);   /* 2*k-1  */
        seq_t factor2 = (seq_t)(5 * k);         /* 5*k    */
        seq_t numerator = factor1 * seq[k-1] - factor2 * seq[k-2];
        seq[k] = numerator / (seq_t)(k - 1);    /* division is exact */
    }
    last = n;

    return seq[n];
}
