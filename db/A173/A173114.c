/*
 * A173114  a(0)=a(1)=1, a(n) = 2*a(n-1)- A010686(n), n>1.
 * https://oeis.org/A173114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173114
   a(0) = 1, a(1) = 1, a(2) = 1
   For n ≥ 3: a(n) = a(n-2) - 2^(n-1)
   (derived from a(n+2) - a(n) = -2^(n+1) in OEIS) */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized values */
static idx_t cache_size = 0;     /* number of valid entries in cache */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0 || n == 1 || n == 2)
        return (seq_t)1;

    /* If we already have the value, return it. */
    if (n < cache_size)
        return cache[n];

    /* Allocate or resize the cache to hold up to index n. */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)malloc(sizeof(seq_t) * new_size);
    if (!new_cache)
        return (seq_t)0;  /* in case of allocation failure; not expected in tests */

    /* Initialize the first three terms. */
    new_cache[0] = new_cache[1] = new_cache[2] = (seq_t)1;

    /* Compute successive terms using the 2‑step recurrence. */
    seq_t twopow = (seq_t)4;          /* 2^(3-1) = 4 for i = 3 */
    for (idx_t i = 3; i <= n; ++i) {
        new_cache[i] = new_cache[i - 2] - twopow;  /* a(i) = a(i-2) - 2^(i-1) */
        twopow = twopow * (seq_t)2;               /* advance 2^k */
    }

    /* Replace old cache with the new one. */
    free(cache);
    cache     = new_cache;
    cache_size = new_size;

    return cache[n];
}
