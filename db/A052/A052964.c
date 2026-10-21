/*
 * A052964  Expansion of (1-x)/((1-2x)(1+x-x^2)).
 * https://oeis.org/A052964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(n+1) + (-1)^n * L(n+2)) / 5 where L(k) are Lucas
 *            numbers (L(0)=2, L(1)=1, L(k)=L(k‑1)+L(k‑2)), equivalently:
 *            a(0)=1, a(1)=0, a(2)=3, a(n+3)=3*a(n+1)+a(n+2)-2*a(n)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   alloc_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_cache.h"

static seq_t *cache = NULL;    /* cache[n] = a(n) */
static idx_t cache_len = 0;    /* number of cached entries (>=0) */

/* Grow the cache to include a(0)…a(n) using the recurrence. */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;                  /* already have the required value */

    idx_t new_len = n + 1;
    seq_t *new_cache = alloc_cache(new_len);   /* allocate array of seq_t */

    /* Copy previously cached values to the new array. */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Initialise the first three terms if the cache was empty. */
    if (cache_len == 0) {
        new_cache[0] = (seq_t)1;
        if (new_len > 1) new_cache[1] = (seq_t)0;
        if (new_len > 2) new_cache[2] = (seq_t)3;
    } else if (cache_len == 1 && new_len > 1) {
        new_cache[1] = (seq_t)0;
    } else if (cache_len == 2 && new_len > 2) {
        new_cache[2] = (seq_t)3;
    }

    /* Compute all terms that are still missing. */
    idx_t start = cache_len;
    if (start < 3)
        start = 3;                 /* need at least three terms to use recurrence */

    for (idx_t i = start; i < new_len; ++i) {
        new_cache[i] =
            (seq_t)3 * new_cache[i - 2]  /* 3*a(i-2) */
          + new_cache[i - 1]             /* + a(i-1) */
          - (seq_t)2 * new_cache[i - 3]; /* - 2*a(i-3) */
    }

    /* Switch to the expanded cache.  The previous cache is left to be freed
       by the caller when desired; for the small test sets it is safe
       to leak the old small block. */
    cache = new_cache;
    cache_len = new_len;
}

/* External entry point required by the framework. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
