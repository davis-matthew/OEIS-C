/*
 * A120507  Generalized meta-Fibonacci sequence a(n) with parameters s=0 and
 *          k=4.
 * https://oeis.org/A120507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n) = n, 2 ≤ n ≤ 4 a(n) = a(n-a(n-1)) + a(n-1-a(n-2))
 *            + a(n-2-a(n-3)) + a(n-3-a(n-4)), n>4 (1‑based indexing)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache of already computed terms (1‑based).  cache[0] is unused. */
static seq_t *cache = NULL;
static idx_t cached = 0;          /* largest n that has been computed */
static size_t cache_cap = 0;      /* current capacity (indices 0..cache_cap) */  

/* Resize cache to hold at least 'need' elements (1‑based). */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)need)
        new_cap <<= 1;  /* double */

    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache)
        return; /* in practice we could abort; here we just return */

    if (cache) {
        memcpy(new_cache, cache, (cached + 1) * sizeof(seq_t));
        free(cache);
    } else {
        /* first time: zero the array up to 'cached', though cached==0 */
        memset(new_cache, 0, (cached + 1) * sizeof(seq_t));
    }

    /* Zero the newly allocated part to keep things clean. */
    if (new_cap > cached)
        memset(new_cache + cached + 1, 0, (new_cap - cached) * sizeof(seq_t));

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute all terms up to n (inclusive).  n >= 1. */
static void compute_up_to(idx_t n)
{
    if (n <= cached)
        return;

    grow_cache(n);
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = 1;
        else if (i <= 4)
            val = (seq_t)i;
        else {
            /* Compute the four indices.  Use seq_t for the subtraction
               to avoid mixing seq_t and idx_t in the same expression. */
            seq_t d1 = (seq_t)i                 - cache[i - 1];
            seq_t d2 = (seq_t)(i - 1)           - cache[i - 2];
            seq_t d3 = (seq_t)(i - 2)           - cache[i - 3];
            seq_t d4 = (seq_t)(i - 3)           - cache[i - 4];

            idx_t t1 = (idx_t)d1;
            idx_t t2 = (idx_t)d2;
            idx_t t3 = (idx_t)d3;
            idx_t t4 = (idx_t)d4;

            /* Sum the four previously computed terms. */
            val = cache[t1] + cache[t2] + cache[t3] + cache[t4];
        }
        cache[i] = val;
    }
    cached = n;
}

/* Public sequence function.  Holds external linkage by default. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* defined only for n>=1; 0 is a safe default */
    compute_up_to(n);
    return cache[n];
}
