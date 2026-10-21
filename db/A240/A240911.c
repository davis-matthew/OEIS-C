/*
 * A240911  Smallest number greater than a(n-1) divisible by smallest number
 *          that is not a divisor of a(n-1).
 * https://oeis.org/A240911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A240911
   a(1) = 1
   For n > 1, a(n) = the smallest integer > a(n−1) that is
   divisible by the smallest integer k ≥ 2 that does not divide a(n−1). */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoisation array (a(1) stored at cache[0]) */
static size_t cache_len = 0;   /* number of elements already computed */
static size_t cache_cap = 0;   /* current allocation size */

/* grow the cache so it can hold at least 'need' elements */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                         /* allocation failure */
    cache      = new_cache;
    cache_cap  = new_cap;
}

/* compute terms up to at least index n */
static void compute_up_to(idx_t n)
{
    if ((size_t)n <= cache_len)        /* already available */
        return;

    if (cache_len == 0) {              /* first call – initialise a(1) */
        ensure_capacity(1);
        cache[0] = (seq_t)1;           /* a(1) = 1 */
        cache_len = 1;
    }

    ensure_capacity((size_t)n);

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev = cache[i - 2];          /* a(i‑1) */

        /* find the smallest k ≥ 2 that does not divide prev */
        idx_t k = 2;
        while ((prev % (seq_t)k) == (seq_t)0)
            ++k;

        /* next term: the smallest > prev divisible by k */
        seq_t next = ((prev / (seq_t)k) + (seq_t)1) * (seq_t)k;
        cache[i - 1] = next;                /* store a(i) */
    }

    cache_len = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined – avoid crash */
        return (seq_t)0;
    compute_up_to(n);
    return cache[n - 1];
}
