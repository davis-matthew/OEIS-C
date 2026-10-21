/*
 * A097742  Pell equation solutions (10*b(n))^2 - 101*a(n)^2 = -1 with b(n) =
 *          A097741(n), n >= 0.
 * https://oeis.org/A097742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 402*a(n-1) - a(n-2), a(0) = 1, a(1) = 401
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises the sequence to handle arbitrary
   ordering of calls while guaranteeing fast O(n) growth.  It uses
   only arithmetic on seq_t, so it works both with fixed-width
   integer types and with the arbitrary-precision class used in
   the C++ build. */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array holding computed values */
static idx_t cached_len = 0;  /* number of entries already in cache */

/* Grow the cache so that it contains values up to index n (inclusive).  */
static void cache_up_to(idx_t n)
{
    if (n < cached_len)
        return;  /* already computed */

    /* Allocate or extend the cache.  Cast the result of realloc to seq_t*.  */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* allocation failed */
    cache = new_cache;

    /* Compute any new terms using the recurrence.  */
    for (idx_t i = cached_len; i <= n; ++i) {
        if (i == 0)
            cache[i] = 1;
        else if (i == 1)
            cache[i] = 401;
        else {
            seq_t tmp = cache[i - 1];
            tmp = tmp * 402;   /* 402 * a(i-1) */
            tmp = tmp - cache[i - 2];
            cache[i] = tmp;
        }
    }
    cached_len = n + 1;
}

seq_t a(idx_t n)
{
    cache_up_to(n);
    return cache[n];
}
