/*
 * A071896  CONTINUANT transform of triangular numbers 1, 3, 6, 10, ...
 * https://oeis.org/A071896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A071896  CONTINUANT transform of triangular numbers 1, 3, 6, 10, …
 *  Recurrence (OEIS convention):
 *     a(1) = 1
 *     a(2) = 4
 *     a(n) = T_n * a(n‑1) + a(n‑2)   (n ≥ 3)
 *  where T_n = n*(n+1)/2  is the n‑th triangular number.  */
#include <stdlib.h>

static seq_t *cache = NULL;        /* 1‑based cache (cache[1] == a(1)) */
static idx_t  cache_limit = 0;     /* largest index that has been computed */

/* Compute all values a(i) for i up to at least n. */
static void compute_up_to(idx_t n)
{
    if (cache_limit >= n)
        return;                       /* already computed */

    /* Grow the cache to hold indices 0 .. n (index 0 unused). */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* allocation failure – terminate */
    cache = new_cache;

    /* Initialise base cases that are still missing. */
    if (cache_limit < 1)
        cache[1] = (seq_t)1;
    if (cache_limit < 2 && n >= 2)
        cache[2] = (seq_t)4;

    /* Compute the missing values (starting at the smallest required index). */
    idx_t start = cache_limit + 1;
    if (start < 3)        /* recurrence starts at n = 3 */
        start = 3;
    for (idx_t i = start; i <= n; ++i) {
        seq_t Ti = ((seq_t)i) * ((seq_t)i + 1) / 2;   /* T_i = i*(i+1)/2 */
        cache[i] = Ti * cache[i - 1] + cache[i - 2];
    }

    cache_limit = n;                /* update the maximal index stored */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* 0 is undefined for this sequence */
    compute_up_to(n);
    return cache[n];
}
