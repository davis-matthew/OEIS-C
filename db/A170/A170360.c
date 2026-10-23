/*
 * A170360  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) satisfies the linear recurrence
 *   a(n) = 13·(a(n-1)+a(n-2)+…+a(n-42)) – 91·a(n-43) + b(n)
 * where the inhomogeneous term b(n) is
 *     b(0) = 1,
 *     b(1..42) = 2,
 *     b(43) = 1,
 *     b(n) = 0 for n ≥ 44.
 *
 * The recurrence is derived from the rational generating function
 * given in the OEIS entry for A170360.
 *
 * The function memoises computed values in a static array so that
 * calls with arbitrary order of indices are answered in amortised
 * O(1) time after an initial O(n) build on the largest requested n.
 */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache   = NULL;   /* array of values a(0)..a(cache_cap-1) */
static idx_t  cache_cap = 0;    /* current allocation size */
static idx_t  cache_size = 0;   /* number of values already computed */

/* Inhomogeneous term b(n) as seq_t */
static inline seq_t b_value(idx_t i)
{
    if (i == 0)      return 1;
    if (i <= 42)     return 2;
    if (i == 43)     return 1;
    return 0;
}

/*
 * Return the n-th term of sequence A170360.
 */
seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (n < cache_size)
        return cache[n];

    /* Ensure enough space for new values */
    if (cache_cap < n + 1) {
        size_t new_cap = (size_t)n + 1;
        if (cache_cap) {
            size_t doubled = cache_cap * 2;
            if (doubled > new_cap)
                new_cap = doubled;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real programme we would handle allocation failure.
               For this task we simply abort. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val = b_value(i);

        /* Sum of the last 42 values multiplied by 13 */
        idx_t lastk = (i < 42) ? i : 42;
        seq_t sum = 0;
        for (idx_t k = 1; k <= lastk; ++k)
            sum += cache[i - k];
        sum *= 13;
        val += sum;

        /* Subtract 91 * a(i-43) if i >= 43 */
        if (i >= 43)
            val -= 91 * cache[i - 43];

        cache[i] = val;
    }

    /* Update the number of computed values */
    cache_size = n + 1;
    return cache[n];
}
