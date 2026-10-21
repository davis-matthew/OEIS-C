/*
 * A099027  a(n) = Sum_{k=0..n} n-k AND NOT k.
 * https://oeis.org/A099027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} n-k AND NOT k Recurrence: a(0) = 0, a(2n) =
 *            2a(n) + 2a(n-1), a(2n+1) = 4a(n) + n + 1 Implementation uses
 *            memoised recursion
 * Range:     u8 n <= 26, u16 n <= 429, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoisation buffer */
static seq_t *cache   = NULL;      /* computed values */
static char *is_set   = NULL;      /* 1 if cache[i] is valid */
static size_t cache_cap = 0;       /* allocated size */

/* Grow the cache so that indices up to need can be stored. */
static void ensure_cache_cap(idx_t need)
{
    if (need < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= need) new_cap <<= 1;
    /* realloc works for both C and C++ if we cast the result. */
    cache   = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    is_set  = (char*)realloc(is_set, new_cap * sizeof(char));
    for (size_t i = cache_cap; i < new_cap; ++i) is_set[i] = 0;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation. */
static seq_t rec(idx_t n)
{
    if (n >= cache_cap) ensure_cache_cap(n);
    if (is_set[n]) return cache[n];

    seq_t result;
    if (n == 0) {
        result = 0;
    } else if (n == 1) {
        result = 1;
    } else if ((n & 1) == 0) {            /* n = 2m */
        idx_t m    = n >> 1;
        seq_t a_m   = rec(m);
        seq_t a_m1  = rec(m - 1);
        result = (seq_t)2 * a_m + (seq_t)2 * a_m1;
    } else {                              /* n = 2m+1 */
        idx_t m    = (n - 1) >> 1;
        seq_t a_m   = rec(m);
        result = (seq_t)4 * a_m + (seq_t)m + 1;
    }
    cache[n] = result;
    is_set[n] = 1;
    return result;
}

/* Required public function */
seq_t a(idx_t n)
{
    return rec(n);
}
