/*
 * A021008  Pisot sequence P(5,11), a(0)=5, a(1)=11, a(n+1) is the nearest
 *          integer to a(n)^2/a(n-1).
 * https://oeis.org/A021008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-4). Initial values: a(0)=5,
 *            a(1)=11, a(2)=24, a(3)=52
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 54, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence.  The array is grown as needed and holds
   the values for all indices that have been computed so far. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored in cache */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Ensure that the cache can hold at least indices 0 .. n.  Increases
   the capacity exponentially, which keeps the overall time complexity
   linear in n and keeps reallocation costs low. */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Compute a(n).  Uses the linear recurrence of order 4 and stores all
   intermediate results in a static cache so that repeated or out‑of‑order
   calls are fast. */
seq_t a(idx_t n)
{
    if (n < 4) {
        /* Handle the first four terms directly.  The cast to seq_t is
           only needed for the built‑in types; for arbitrary‑precision
           classes it is harmless. */
        if (n == 0) return (seq_t)5;
        if (n == 1) return (seq_t)11;
        if (n == 2) return (seq_t)24;
        return (seq_t)52;               /* n==3 */
    }

    if (n >= cache_size) {
        ensure_cache(n);

        /* If cache is empty, initialise the first four values. */
        if (cache_size == 0) {
            cache[0] = (seq_t)5;
            cache[1] = (seq_t)11;
            cache[2] = (seq_t)24;
            cache[3] = (seq_t)52;
            cache_size = 4;
        }

        /* Extend the cache up to n using the recurrence. */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* a(i) = 3*a(i-1) - 2*a(i-2) + a(i-4)   for i>=4 */
            cache[i] =
                (seq_t)3 * cache[i-1] -
                (seq_t)2 * cache[i-2] +
                cache[i-4];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
