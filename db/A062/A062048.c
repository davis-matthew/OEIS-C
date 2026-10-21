/*
 * A062048  a(n) = Sum_{k=1..n} floor(sqrt(prime(k))).
 * https://oeis.org/A062048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(sqrt(prime(k)))
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt, get_prime, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by caching partial sums so that calls in
   arbitrary order are efficient. */

#include <stddef.h>                         /* for size_t              */
#include "../utils/get_prime.h"             /* uint32_t get_prime(idx_t k)   */
#include "../utils/floor_sqrt.h"           /* seq_t floor_sqrt(seq_t x) */
#include "../utils/grow_seq.h"             /* void grow_seq(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* Behaviour below the offset is irrelevant. */
    if (n < 1) return (seq_t)0;

    /* Cached cumulative sums: cache[i] = a(i). */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  cached_n  = 0;   /* largest n that has been computed */

    /* If we already have the requested value, return it immediately. */
    if (n <= cached_n) return cache[n];

    /* Make sure the cache can hold indices up to n. */
    if (cache_cap < (size_t)(n + 1))
        grow_seq(&cache, &cache_cap, (size_t)(n + 1));

    /* Initialise the zeroth element only once. */
    if (cached_n == 0) cache[0] = (seq_t)0;

    /* Extend the cache up to the requested n. */
    for (idx_t i = cached_n + 1; i <= n; ++i)
    {
        uint32_t p  = get_prime(i);          /* k‑th prime (1‑based)      */
        seq_t    sq = floor_sqrt((seq_t)p);  /* ⌊√p⌋                      */
        cache[i] = cache[i - 1] + sq;        /* cumulative sum             */
    }

    cached_n = n;
    return cache[n];
}
