/*
 * A170033  Number of reduced words of length n in Coxeter group on 24
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the linear recurrence  
 *   a(0) = 1  
 *   for 1 ≤ n ≤ 35:  a(n) = 22 * Σ_{i=1}^{n} a(n-i) + 2  
 *   for n = 36:      a(36) = 22 * Σ_{i=1}^{35} a(36-i) + 1 − 253·a(0)  
 *   for n ≥ 37:      a(n) = 22 * Σ_{i=1}^{35} a(n-i) − 253·a(n-36)  
 * (All sums include only existing indices.)
 * The function memoises computed values to provide O(n) time per call for
 * n up to ~10 000 and keeps all intermediate arithmetic in the native seq_t type.
 */

#include <stdlib.h>

/* Return the n-th term of the A170033 sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoisation array                   */
    static size_t cap   = 0;      /* currently allocated capacity        */
    static idx_t  size  = 0;      /* number of values currently stored   */

    /* If the requested value is already computed, return it. */
    if (n < size)
        return cache[n];

    /* ---- grow the allocation if necessary ---- */
    if (cache == NULL) {
        cap = 64;                     /* initial capacity                        */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) {                 /* allocation failure                      */
            /* In a real environment we might abort or handle this, but returning 0 */
            return (seq_t)0;
        }
        size = 0;
    }
    /* Ensure capacity for indices up to 'n' */
    while ((size_t)(n + 1) > cap) {
        size_t new_cap = cap << 1;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                     /* reallocation failed */
            return (seq_t)0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* ---- compute missing terms ---- */
    for (idx_t i = size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else {
            /* Sum of at most 35 previous terms */
            idx_t kmax = (i < 36) ? i : 35;
            seq_t sumprev = (seq_t)0;
            for (idx_t k = 1; k <= kmax; ++k)
                sumprev += cache[i - k];

            sumprev *= (seq_t)22;                           /* multiply by 22 */

            seq_t constant = (seq_t)0;
            if (i <= 35)      constant = (seq_t)2;
            else if (i == 36) constant = (seq_t)1;         /* b_36 */

            val = sumprev + constant;

            if (i >= 36)      /* subtract 253 * a[i-36] for n ≥ 36 */
                val -= cache[i - 36] * (seq_t)253;
        }
        cache[i] = val;
    }
    size = n + 1;          /* update size after filling up to n */

    return cache[n];
}
