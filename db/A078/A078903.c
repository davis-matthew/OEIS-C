/*
 * A078903  a(n) = n^2 - Sum_{u=1..n} Sum_{v=1..u} valuation(2*v, 2).
 * https://oeis.org/A078903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - Sum_{k=1..n} v2(k) Recurrence (for n ≥ 3): a(2*n) =
 *            a(n) + a(n-1) + (n-1) a(2*n+1) = 2*a(n) + n Base values: a(1) =
 *            0, a(2) = 0 Cache is used so that repeated calls of any order
 *            run in O(log n)
 * Range:     u8 n <= 110, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
static seq_t *cache = NULL;     /* cache array, cache[0] unused, cache[1]..cache[n] contain results */
static size_t cache_size = 0;   /* current allocated size (cache_size >= n+1 when accessed) */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* we need indices 0..n inclusive */
    if (cache_size < needed) {
        seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        /* initialize newly allocated slots to 0 */
        for (size_t i = cache_size; i < needed; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = needed;
    }
}

/* Recursive helper that computes a(n) using the recurrence and memoization. */
static seq_t a_impl(idx_t n)
{
    if (n <= 2) return (seq_t)0;           /* a(1)=a(2)=0 */
    if (cache[n] != (seq_t)0) return cache[n];

    seq_t result;
    if ((n & 1) == 0) {                    /* n is even: n = 2*m */
        idx_t m = n >> 1;
        seq_t am  = (cache[m] != (seq_t)0) ? cache[m] : a_impl(m);
        seq_t am1 = (m == 1) ? (seq_t)0 : ((cache[m-1] != (seq_t)0) ? cache[m-1] : a_impl(m-1));
        result = am + am1 + (seq_t)(m - 1);
    } else {                              /* n is odd: n = 2*m+1 */
        idx_t m = n >> 1;                  /* (n-1)/2 */
        seq_t am = (cache[m] != (seq_t)0) ? cache[m] : a_impl(m);
        result = 2 * am + (seq_t)m;
    }
    cache[n] = result;
    return result;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset but safe */
    ensure_cache(n);
    return a_impl(n);
}
