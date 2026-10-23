/*
 * A033649  Trajectory of 5 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 46, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 5; for n > 0, a(n) = a(n-1) + rev_digits(a(n-1))
   where rev_digits(x) is the decimal digit reversal of x. */

#include <stdlib.h>
#include <stddef.h>

static inline seq_t reverse_digits(seq_t x)
{
    seq_t rev = (seq_t)0;
    while (x)
    {
        rev = rev * (seq_t)10 + x % (seq_t)10;
        x = x / (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    /* Static cache for memoization: cache[i] holds a(i) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored */

    const seq_t start_val = (seq_t)5;   /* a(0) */

    /* Initialize cache on first call */
    if (!cache)
    {
        cap = 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = start_val;
        len = 1;
    }

    /* Expand cache capacity if needed */
    size_t n_sz = (size_t)n;
    if (n_sz >= cap)
    {
        size_t newcap = cap;
        if (newcap == 0) newcap = 1;
        while (newcap <= n_sz)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cap = newcap;
    }

    /* Compute terms up to index n */
    for (size_t i = len; i <= n_sz; ++i)
    {
        cache[i] = cache[i-1] + reverse_digits(cache[i-1]);
    }
    len = n_sz + 1;

    return cache[n_sz];
}
