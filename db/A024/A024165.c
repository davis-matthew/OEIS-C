/*
 * A024165  Number of integer-sided triangles with sides a,b,c, a<b<c, a+b+c=n
 *          such that c - b > b - a.
 * https://oeis.org/A024165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) + a(n-6) - a(n-7) - a(n-9) - a(n-10) +
 *            a(n-13) for n >= 14, with a(1) … a(12) = 0, a(13) = 1
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers for memory handling */
#include <stdlib.h>
#include <string.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed values, index 0 … n */
static idx_t  cache_len = 0;  /* current length of the cache (n+1) */

/* Ensure that cache contains values up to index n */
static void
ensure_cache(idx_t n)
{
    if (cache_len > 0 && cache_len > n)
        return;                      /* already computed up to n */

    /* Allocate a new cache array of size n+1 */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache)                 /* allocation failure – abort */
        return;                     /* caller will see 0 in cache */

    /* Initialise all entries to zero */
    memset(new_cache, 0, new_len * sizeof(seq_t));

    /* Base value: a(13) = 1  (indices <13 are already 0) */
    if (n >= 13)
        new_cache[13] = (seq_t)1;

    /* Compute subsequent values using the recurrence */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t val = new_cache[i - 3] + new_cache[i - 4] + new_cache[i - 6];
        val -= new_cache[i - 7];
        val -= new_cache[i - 9];
        val -= new_cache[i -10];
        val += new_cache[i -13];
        new_cache[i] = val;
    }

    /* Replace old cache, freeing memory if needed */
    if (cache)
        free(cache);
    cache      = new_cache;
    cache_len  = new_len;
}

/* Public function: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* sequence is defined for n >= 1 only */

    ensure_cache(n);
    return cache[n];
}
