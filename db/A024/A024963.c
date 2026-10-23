/*
 * A024963  a(n) = Sum_{i=1..floor((n+2)/4)} a(2i-1)*a(n-2i+1), with a(1)=3,
 *          a(2)=1, and a(3)=2.
 * https://oeis.org/A024963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..⌊(n+2)/4⌋} a(2i-1)*a(n-2i+1) a(1)=3, a(2)=1,
 *            a(3)=2 Cache the values so that every call is O(n)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache for memoised values */
static seq_t *seq_cache = NULL;   /* array indexed from 1 (index 0 unused) */
static idx_t   cache_cap  = 0;    /* allocated size (excluding index 0) */
static idx_t   cache_limit = 0;   /* largest n for which a(n) is stored */

/* Ensure the cache has space for indices up to n.  Grow exponentially. */
static void ensure_cache(idx_t n)
{
    if (cache_cap >= n)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap < n)
        new_cap <<= 1;
    /* In C++ a cast is needed for the void* returned by realloc   */
    seq_t *tmp = (seq_t *)realloc(seq_cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp)
        abort();                 /* unrecoverable allocation failure */
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) with memoisation and incremental building. */
seq_t a(idx_t n)
{
    if (n < 1)                       /* undefined for n < 1 */
        return (seq_t)0;

    ensure_cache(n);

    /* Initialise base value a(1) on first use.  Higher bases are
       filled lazily during the incremental loop. */
    if (cache_limit == 0) {
        seq_cache[1] = (seq_t)3;
        cache_limit = 1;
    }

    /* Compute values from (cache_limit+1) up to n */
    for (idx_t m = cache_limit + 1; m <= n; ++m) {
        if (m == 1) {
            seq_cache[m] = (seq_t)3;          /* should not be reached */
        } else if (m == 2) {
            seq_cache[m] = (seq_t)1;
        } else if (m == 3) {
            seq_cache[m] = (seq_t)2;
        } else {
            idx_t k = (m + 2) / 4;            /* floor((m+2)/4) */
            seq_t sum = 0;
            for (idx_t i = 1; i <= k; ++i) {
                seq_t prod = seq_cache[2*i-1] * seq_cache[m - 2*i + 1];
                sum += prod;
            }
            seq_cache[m] = sum;
        }
    }

    cache_limit = n;
    return seq_cache[n];
}
