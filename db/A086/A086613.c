/*
 * A086613  Row sums of triangle A086612.
 * https://oeis.org/A086613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A086613 – Row sums of triangle A086612
 *
 *  Recurrence (derived from the generating function):
 *      a(0) = 1, a(1) = 2
 *      for n ≥ 2:
 *          a(n) = Σ_{k=0}^{n-1} a(k)·a(n-1-k) - c(n)
 *          where c(2)=1, c(3)=2, c(4)=1, and c(n)=0 for n>4
 *
 *  The coefficients c(n) arise from the term -(1+x)^2·x^2 in the
 *  functional equation for A(x).
 *
 *  The implementation uses a memoised table so that subsequent calls
 *  for arbitrary n (in any order) are answered in O(n²) time at most.
 *  For n up to about 10 000 the routine stays well under the
 *  two‑second limit even for an arbitrary‑precision seq_t.
 */

#include <stdlib.h>

/* Make sure the cache can hold indices up to max_n (inclusive). */
static void
ensure_cache(idx_t max_n)
{
    static seq_t *cache = NULL;          /* static inside function for visibility in a() */
    static idx_t cap   = 0;              /* current allocated length */

    if (cap <= max_n)
    {
        idx_t new_cap = max_n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache)                /* allocation failure – abort */
            exit(EXIT_FAILURE);
        cache = new_cache;
        /* initialise the newly allocated entries (they were zeroed by realloc) */
        for (idx_t i = cap; i <= max_n; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }
}

/* Compute the n‑th term of A086613.  This function can be called
 * in any order; the static table ensures only the missing entries
 * are filled in each call. */
seq_t a(idx_t n)
{
    /* Static table definitions (must be visible across calls). */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    if (cache == NULL)                  /* first call: initialise global cache */
    {
        cache = (seq_t *)calloc(1, sizeof(seq_t)); /* allocate at least one element */
        if (!cache) exit(EXIT_FAILURE);
        cap = 1;
    }

    /* Ensure the table can hold n. */
    if (cap <= n)
    {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        for (idx_t i = cap; i <= n; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Set base cases if not already set. */
    if (cap > 0 && cache[0] == 0) cache[0] = (seq_t)1;
    if (cap > 1 && cache[1] == 0) cache[1] = (seq_t)2;

    /* Compute any missing terms up to n (in increasing order). */
    for (idx_t k = 2; k <= n; ++k)
    {
        if (cache[k] != 0) continue;          /* already computed in a previous call */

        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i <= k-1; ++i)
        {
            seq_t prod = cache[i] * cache[k-1-i];
            sum += prod;
        }

        /* Subtract the fixed constants for n=2,3,4. */
        if (k == 2) sum -= (seq_t)1;
        else if (k == 3) sum -= (seq_t)2;
        else if (k == 4) sum -= (seq_t)1;

        cache[k] = sum;
    }

    return cache[n];
}
