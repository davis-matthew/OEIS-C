/*
 * A169426  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A169426 – Number of reduced words of length n in a Coxeter group
 *  with 29 generators satisfying S_i^2 = (S_i S_j)^32 = 1.
 *
 *  The ordinary generating function is
 *      (1 + 2*Σ_{k=1}^{31} t^k + t^32) / (1 - 27*Σ_{k=1}^{31} t^k + 378*t^32).
 *
 *  From the denominator a linear recurrence follows:
 *      a(0) = 1
 *      a(n) = 27*(a(n-1)+…+a(n-31)) - 378*a(n-32) + p(n)
 *      where p(0)=1, p(1)…p(31)=2, p(32)=1, and p(n)=0 for n>32.
 *
 *  The function below iteratively computes and memoises the sequence,
 *  so that calls may arrive in any order.  The implementation uses only
 *  integer arithmetic and basic library facilities to satisfy the
 *  C11 / C++17 constraints.
 */

#include <stdlib.h>

/* memoisation buffer */
static seq_t *cache = NULL;   /* sequence values */
static size_t cache_cap = 0;  /* capacity of the buffer */
static size_t cache_len = 0;  /* number of values stored (≥1 when cache_len>0) */

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Return a cached value if possible. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Make sure the buffer is large enough. */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                  /* grow geometrically */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – caller may handle the error. */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute the current sum of the last 31 terms (or fewer if not yet).
     * This 'window_sum' will be used to evaluate
     * 27 * (a(m-1)+…+a(m-31)) for each new term. */
    seq_t window_sum = 0;
    size_t start = (cache_len >= 31) ? cache_len - 31 : 0;
    for (size_t i = start; i < cache_len; ++i)
        window_sum += cache[i];

    /* Iterate from the first missing index up to the requested n. */
    for (size_t m = cache_len; m <= (size_t)n; ++m) {
        seq_t term = (seq_t)27 * window_sum;          /* 27*Σ_{1..31} a(m-k) */

        if (m >= 32)
            term -= (seq_t)378 * cache[m - 32];      /* –378*a(m-32)       */

        /* Add the numerator contribution p(m). */
        if (m == 0)
            term += 1;                                 /* p(0)=1 */
        else if (m <= 31)
            term += 2;                                 /* p(1…31)=2 */
        else if (m == 32)
            term += 1;                                 /* p(32)=1 */

        cache[m] = term;

        /* Update the sliding window for the next coefficient. */
        window_sum += term;                            /* include a(m)   */
        if (m >= 31)                                    /* keep only 31 terms */
            window_sum -= cache[m - 31];
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
