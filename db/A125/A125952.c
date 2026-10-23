/*
 * A125952  a(0)=1. a(n) = sum{k=0 to n-1} a(a(k)(mod n)).
 * https://oeis.org/A125952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; a(n)=∑_{k=0}^{n-1} a(a(k) mod n)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static state: cache of computed values and its bounds. */
static seq_t *cache = NULL;
static idx_t cap   = 0;  /* allocated capacity (in indices) */
static idx_t last  = 0;  /* highest n that has been computed */

/* Make sure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n >= cap) {
        idx_t newcap = (cap > 0) ? cap : 1;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }
}

/* Compute a(n) with memoisation; handles calls in any order. */
seq_t a(idx_t n)
{
    /* First call: initialise a(0)=1. */
    if (cache == NULL) {
        ensure_cache(0);
        cache[0] = (seq_t)1;
        last = 0;
    }

    if (n <= last)
        return cache[n];

    /* Compute all a(i) for i = last+1 … n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        ensure_cache(i);
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t r = cache[k] % (seq_t)i;          /* a(k) mod i */
            idx_t idx = (idx_t)r;                    /* safe: r < i ≤ n */
            sum += cache[idx];
        }
        cache[i] = sum;
    }
    last = n;
    return cache[n];
}
