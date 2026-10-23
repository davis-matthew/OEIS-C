/*
 * A180046  a(n+1) = a(n-3) + a(n-2) - a(n-1) + a(n) starting with 1, 2, 3, 4.
 * https://oeis.org/A180046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+1) = a(n-3) + a(n-2) - a(n-1) + a(n) Initial values: a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4
 * Range:     u8 n <= 20, u16 n <= 42, u32 n <= 86, u64 n <= 172,
 *            u128 n <= 346, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the nth term of OEIS sequence A180046.
   It uses a simple O(n) memoised recurrence to answer calls
   in any order.  The sequence grows rapidly; the implementation
   is safe for any build that provides a sufficient integer type
   (uint8_t … uint64_t, unsigned __int128 or arbitrary precision). */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array indexed from 1 */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* last filled index (0 means none) */

/* Ensure cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1; /* we ignore index 0 */
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();          /* allocation failure */
    cache_cap = new_cap;

    /* initialise base values if this is the first allocation */
    if (cache_len == 0) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cache_len = 4;
    }
}

/* Compute a(n) using memoised recurrence. */
seq_t a(idx_t n)
{
    if (n <= 4u) {                /* bounds are unsigned, safe */
        switch ((unsigned)n) {
            case 1: return (seq_t)1;
            case 2: return (seq_t)2;
            case 3: return (seq_t)3;
            case 4: return (seq_t)4;
        }
    }

    ensure_capacity(n);

    /* Compute values up to n if not already done. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* a(i) = a(i-4) + a(i-3) - a(i-2) + a(i-1) */
        seq_t val =
            cache[i-4] + cache[i-3] - cache[i-2] + cache[i-1];
        cache[i] = val;
    }
    cache_len = (size_t)n; /* last filled index */

    return cache[n];
}
