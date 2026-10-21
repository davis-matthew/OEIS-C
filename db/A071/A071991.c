/*
 * A071991  a(1) = a(2) = 1; a(n) = a(floor(n/3)) + a(n - floor(n/3)).
 * https://oeis.org/A071991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(floor(n/3)) + a(n - floor(n/3)), a(1)=a(2)=1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Memoised recursive implementation.  The cache is an array indexed
 *  from 1 to n.  We allocate 0‐based and never use cache[0] which
 *  keeps the sentinel value 0 (meaning “not computed yet”) for all
 *  uninitialised entries.  The array is grown to at least (n+1) slots
 *  (so that indices 1…n are valid) and enlarges by doubling when,
 *  again, a larger index is required.  All operations are performed
 *  using the supplied integer types seq_t and idx_t; no overflow can
 *  occur because the result fits in seq_t as required by the build. */
#include <stdlib.h>
#include <stdint.h>   /* for abort() */

static seq_t *cache = NULL;      /* dynamic memoisation table */
static idx_t cache_size = 0;     /* current number of allocated slots */

/* Ensure cache has room for index n (inclusive).  The array is
 * indexed 0…cache_size-1 but we only use indices 1…n; cache[0] is
 * unused and left unused (it keeps the value 0). */
static void ensure_cache(idx_t n)
{
    /* Need at least n+1 slots so that cache[n] is valid.  keep
     * cache_size > 0 so that later accesses to cache[1] or
     * cache[2] are within bounds. */
    if (n + 1 <= cache_size)
        return;

    idx_t new_size = cache_size > 0 ? cache_size * 2 : 2;
    while (new_size <= n + 1)
        new_size *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();  /* out of memory */

    /* Initialise newly allocated entries to 0 (uncomputed). */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_size = new_size;
}

/* Recursive helper with memoisation. */
static seq_t a_rec(idx_t n)
{
    /* Return cached value if present. */
    if (cache[n] != 0)
        return cache[n];

    /* Base cases */
    if (n == 1 || n == 2) {
        cache[n] = 1;
        return 1;
    }

    idx_t m = n / 3;                     /* floor(n/3) */
    seq_t val = a_rec(m) + a_rec(n - m);
    cache[n] = val;
    return val;
}

/* External entry point */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                       /* undefined for non‑positive indices */

    ensure_cache(n);
    return a_rec(n);
}
