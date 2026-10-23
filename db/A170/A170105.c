/*
 * A170105  Number of reduced words of length n in Coxeter group on 48
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   a(n) = 46*Σ_{k=1}^{36} a(n-k) - 1081*a(n-37) + c_n
   where c_n = 2 for 1≤n≤36, c_n = 1 for n=37, otherwise c_n = 0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation cache for previously computed terms */
    static seq_t *buf = NULL;          /* array of a(0)..a(last_computed)   */
    static size_t capacity = 0;         /* allocated number of elements      */
    static idx_t last_computed = 0;     /* highest index that has been filled */
    static int  initialized = 0;        /* 0 = first call, 1 = ready       */

    /* Handle the first call */
    if (!initialized)
    {
        /* Allocate at least one element; we may allocate more now to avoid
           extra reallocations if the first request is large. */
        size_t init_cap = (n > 0) ? (size_t)(n + 1) : 1;
        buf = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!buf) return (seq_t)0;          /* allocation failure */
        capacity = init_cap;
        buf[0] = (seq_t)1;                   /* a(0) = 1 */
        last_computed = 0;
        initialized = 1;
        if (n == 0) return buf[0];
    }

    /* Grow the cache if necessary */
    if ((size_t)(n + 1) > capacity)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        buf      = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    if (n > last_computed)
    {
        for (idx_t i = last_computed + 1; i <= n; ++i)
        {
            seq_t sum = (seq_t)0;

            /* 46 * Σ_{k=1}^{36} a(i-k)   */
            for (idx_t k = 1; k <= 36 && i >= k; ++k)
                sum += buf[(size_t)(i - k)] * (seq_t)46;

            /* -1081 * a(i-37) if i ≥ 37 */
            if (i >= 37)
                sum -= buf[(size_t)(i - 37)] * (seq_t)1081;

            /* Add the inhomogeneous part c_i */
            seq_t c = (seq_t)0;
            if (i <= 36)   c = (seq_t)2;
            else if (i == 37) c = (seq_t)1;
            /* otherwise c stays 0 */

            buf[(size_t)i] = sum + c;
        }
        last_computed = n;
    }

    return buf[(size_t)n];
}
