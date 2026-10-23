/*
 * A159769  Number of n-leaf binary trees that do not contain
 *          (((()())())(()(()()))) as a subtree.
 * https://oeis.org/A159769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((9n-3)*a(n-1) + (4n-14)*a(n-4) - (4n-2)*a(n-2) -
 *            (8n-28)*a(n-3)) / (2*(n+1)), with a(1)=1, a(2)=1, a(3)=2, a(4)=5
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>  /* size_t */

/* cache for memoisation (indices start at 1) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_up_to = 0;

/* grow cache to accommodate indices up to 'needed' */
static void ensure_cache(idx_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= (size_t)needed)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache && new_cap > 0)
        exit(EXIT_FAILURE);
    cache = new_cache;

    /* initialise newly allocated cells to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not part of the sequence */

    /* initialise base cases on first call */
    if (!cache) {
        ensure_cache(4);
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)5;
        cached_up_to = 4;
    }

    if (n <= cached_up_to)
        return cache[n];

    ensure_cache(n);

    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t term1 = ((seq_t)(9 * i - 3)) * cache[i - 1];
        seq_t term2 = ((seq_t)(4 * i - 14)) * cache[i - 4];
        seq_t term3 = ((seq_t)(4 * i - 2)) * cache[i - 2];
        seq_t term4 = ((seq_t)(8 * i - 28)) * cache[i - 3];

        /* accumulate positive and negative contributions separately
           to avoid unsigned underflow.  By the recurrence, the
           positive part is always >= the negative part.          */
        seq_t pos = term1 + term2;
        seq_t neg = term3 + term4;
        seq_t numerator = pos >= neg ? pos - neg : neg - pos;   /* safety */

        seq_t denom = (seq_t)(2 * (i + 1));
        cache[i] = numerator / denom;                          /* exact division */
    }

    cached_up_to = n;
    return cache[n];
}
