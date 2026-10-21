/*
 * A091903  Expansion of x/((1+5*x)*(1-10*x)).
 * https://oeis.org/A091903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 50*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n‑th term of OEIS sequence A091903.
   Uses dynamic memoisation so calls may be in any order. */
seq_t a(idx_t n)
{
    /* Handle trivial cases immediately. */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;

    /* Static cache for memoisation. */
    static seq_t *cache   = NULL;      /* pointer to stored terms */
    static size_t capacity = 0;        /* allocated array size    */
    static idx_t  last     = 1;        /* highest n that has been computed */

    /* Allocate initial storage for a(0) and a(1) if not yet done. */
    if (capacity == 0) {
        capacity = 2;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* silently fail on OOM */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow the cache array if it cannot hold up to n. */
    if ((idx_t)(capacity - 1) < n) {
        size_t newcap = capacity;
        while ((idx_t)(newcap - 1) < n) newcap = newcap * 2 + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* silently fail on OOM */
        cache = tmp;
        capacity = newcap;
    }

    /* Compute any missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = 5*a(i-1) + 50*a(i-2) */
        cache[i] = cache[i-1] * (seq_t)5 + cache[i-2] * (seq_t)50;
    }
    last = n;

    return cache[n];
}
