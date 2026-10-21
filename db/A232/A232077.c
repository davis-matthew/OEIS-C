/*
 * A232077  Number of (1+1) X (n+1) 0..1 arrays with every element equal to
 *          some horizontal, diagonal or antidiagonal neighbor, with top left
 *          element zero.
 * https://oeis.org/A232077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + 4*a(n-3) + 4*a(n-4), with initial
 *            values a(1)=3, a(2)=15, a(3)=46, a(4)=161
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;    /* 1‑based cache, cache[0] unused */
static size_t cache_len = 0;    /* largest index that has been computed */

/* Allocate or grow the cache so that it can hold indices up to n inclusive.
   Initialise the four base values whenever the cache grows to cover them. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_len) return;          /* already present */

    /* new_cap is the largest index we want to store (0 .. new_cap) */
    size_t new_cap = (size_t)n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();                           /* allocation failure */
    cache = tmp;

    /* Ensure the four base terms are set if the cache now covers them. */
    if (new_cap >= 1) cache[1] = (seq_t)3;
    if (new_cap >= 2) cache[2] = (seq_t)15;
    if (new_cap >= 3) cache[3] = (seq_t)46;
    if (new_cap >= 4) cache[4] = (seq_t)161;

    /* Compute any missing terms using the recurrence.  The recurrence is
       defined for n >= 5, so start the loop at max(5, cache_len+1). */
    size_t start = cache_len + 1;
    if (start < 5) start = 5;
    for (size_t i = start; i <= new_cap; ++i) {
        cache[i] =
            cache[i - 1] * (seq_t)3
          + cache[i - 2]
          + cache[i - 3] * (seq_t)4
          + cache[i - 4] * (seq_t)4;
    }

    cache_len = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1, but harmless */
    ensure_cache(n);
    return cache[(size_t)n];
}
