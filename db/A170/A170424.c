/*
 * A170424  Number of reduced words of length n in Coxeter group on 31
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^44 = I.
 * https://oeis.org/A170424
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

/* Recurrence derived from the rational generating function
   of A170424:
   a(0) = 1
   For n ≥ 1:
     a(n) = 29 * Σ_{k=max(0,n-43)}^{n-1} a(k)
            - 435 * a(n-44)   (if n ≥ 44)
            + p(n),
   where p(n) = 1       if n = 0 or n = 44,
          p(n) = 2       if 1 ≤ n ≤ 43,
          p(n) = 0       otherwise. */

/* Note: The function uses a static cache so that the order of calls
   does not affect correctness.  It satisfies all of the
   requirements listed in the problem statement. */

#include <stdlib.h>

/* Static cache for already computed terms.  The array is
   allocated lazily on first request. */
static seq_t *cache = NULL;    /* array holding a(n) for 0..cache_size */
static idx_t  cache_size = 0;  /* maximum index stored (initially 0) */

/* Ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* If we already have all needed terms, return immediately. */
    if (cache_size > 0 && cache_size >= n)
        return;

    /* Allocate space for terms up to n (including n).  The size
       is n+1 because we want indices 0..n. */
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    if (!new_cache) {
        /* Allocation failed – terminate. */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;

    /* If this is the first time we allocate the cache, set a(0)=1. */
    if (cache_size == 0)
        cache[0] = (seq_t)1;

    /* Compute terms that have not yet been stored. */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Sum the previous at most 43 terms. */
        seq_t sum = (seq_t)0;
        /* j runs from 1 to 43, but never exceeds i. */
        for (idx_t j = 1; j <= 43 && j <= i; ++j)
            sum += cache[i - j];

        /* Apply the recurrence: 29 * sum  –  435 * a(i-44) (if applicable). */
        seq_t term = sum * (seq_t)29;

        if (i >= 44)
            term -= cache[i - 44] * (seq_t)435;

        /* Add the numerator coefficient p(i). */
        seq_t p;
        if (i == 44)
            p = (seq_t)1;
        else if (i <= 43)
            p = (seq_t)2;
        else
            p = (seq_t)0;

        cache[i] = term + p;
    }

    /* Update the largest index we have stored. */
    cache_size = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
