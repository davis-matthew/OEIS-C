/*
 * A102840  a(0)=0, a(1)=1, a(n)=((2*n-1)*a(n-1)-5*n*a(n-2))/(n-1).
 * https://oeis.org/A102840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 0, a(1) = 1, a(n) = ((2*n-1)*a(n-1) - 5*n*a(n-2)) / (n-1)
 *            for n >= 2
 * Range:     i8 n <= 6, i16 n <= 12, i32 n <= 26, i64 n <= 52, i128 n <= 107,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 866 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *seq   = NULL;
    static idx_t cap    = 0;   /* allocated capacity */
    static idx_t last   = 0;   /* highest index already computed */

    /* Ensure capacity for at least n+1 elements, but at least 2 for the
       initial values a_wide(0) and a_wide(1). */
    idx_t needed = n + 1;
    if (needed < 2) needed = 2;
    if (needed > cap) {
        seq_t *tmp = mem_resize(seq_t, seq, needed);
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

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}
