/*
 * A170152  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170152
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

/* A170152: number of reduced words of length n in the Coxeter group on 47
   generators with relations (S_i)^2 = (S_i S_j)^38 = I.
   The generating function is
     (t^38 + 2*t^37 + … + 2*t + 1) /
     (1035*t^38 - 45*t^37 - … - 45*t + 1).
   The corresponding linear recurrence for n ≥ 0 is
     a(n) = 45 ⋅ (a(n‑1)+…+a(n‑37)) – 1035 ⋅ a(n‑38) + c_n,
   where the inhomogeneous term c_n comes from the numerator:
     c_0 = 1,   c_n = 2 for 1 ≤ n ≤ 37,   c_38 = 1,   c_n = 0 for n ≥ 39.
   In the notation used in the OIES repository the function is defined
   as:
   seq_t a(idx_t n);   (idx_t = uint64_t, seq_t is the build‑provided
   integer type).  The implementation below memoises all computed
   values and computes new values only up to the requested index.  */


/* Standard headers. The cast from void* to seq_t* is required for both C11
   and C++17. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>   /* for uint64_t in case builds use it */

/* External integer types supplied by the build system. */
extern seq_t a(idx_t n);

/* Implementation of the function. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated capacity (number of elements) */
    static idx_t computed = 0;      /* how many terms have been computed (>=0) */

    /* Grow the cache if necessary.  The growth factor 2 keeps the
       amortised complexity O(1). */
    if ((size_t)(n + 1) > cache_size) {
        size_t new_size = cache_size > 0 ? cache_size : 1;
        while (new_size <= (size_t)(n + 1)) {
            new_size <<= 1;          /* double the size until it fits */
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing terms up to index n (inclusive). */
    while (computed <= n) {
        idx_t i = computed;
        seq_t sum = 0;

        /* Sum of the last 37 terms: a(i‑1) … a(i‑37).  For i < 37 some
           terms are missing; a(j) with j < 0 is treated as 0 by the
           condition below. */
        for (idx_t k = 1; k <= 37; ++k) {
            if (i >= k)
                sum += cache[i - k];
        }

        /* 45 ⋅ sum of the previous 37 terms. */
        seq_t term = (seq_t)45 * sum;

        /* Subtract 1035 ⋅ a(i‑38) if i ≥ 38. */
        if (i >= 38) {
            term -= (seq_t)1035 * cache[i - 38];
        }

        /* Add the inhomogeneous part c_i. */
        seq_t coeff = 0;
        if (i == 0 || i == 38) {
            coeff = (seq_t)1;
        } else if (i <= 37) {
            coeff = (seq_t)2;
        }
        term += coeff;

        /* Store the result and move to the next index. */
        cache[i] = term;
        ++computed;
    }

    /* Return the requested term. */
    return cache[n];
}
