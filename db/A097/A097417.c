/*
 * A097417  a(1)=1; a(n+1) = Sum_{k=1..n} a(k) a(floor(n/k)).
 * https://oeis.org/A097417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; a(n+1)=∑_{k=1..n} a(k) * a(floor(n/k))
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Memoisation cache shared by all calls.
   cache[i] contains a(i); cache[0] unused.
   cache_capacity is the allocated size (≥ n+1). */
static seq_t *cache = NULL;      /* allocated array of a(i) */
static size_t cache_capacity = 0;
static idx_t last_computed = 0;   /* highest index that has been computed */

/* Ensure the cache can hold indices 0 .. n. */
static void ensure_cache(idx_t n)
{
    if (cache_capacity > (size_t)n)
        return;

    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp)
        abort();                     /* out of memory */
    /* Zero‑initialise the new portion.  For unsigned integer types
       this yields the value 0. */
    if (tmp != cache)
        memset(tmp + cache_capacity, 0, sizeof(seq_t) * (new_cap - cache_capacity));

    cache = tmp;
    cache_capacity = new_cap;
}

/* Compute a(n).  The function is safe for any order of calls
   thanks to memoisation.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* a(0) is undefined; return 0 for safety */

    if (n <= last_computed)          /* already computed */
        return cache[(size_t)n];

    ensure_cache(n);

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        if (i == (idx_t)1) {          /* a(1) = 1 */
            cache[(size_t)i] = (seq_t)1;
            continue;
        }

        idx_t m = i - 1;              /* floor(n/k) uses m = i-1 */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= m; ++k)
        {
            idx_t f = m / k;          /* floor((i-1)/k) */
            sum += cache[(size_t)k] * cache[(size_t)f];
        }
        cache[(size_t)i] = sum;
    }

    last_computed = n;
    return cache[(size_t)n];
}
