/*
 * A157727  a(n) = Fibonacci(n) + 4.
 * https://oeis.org/A157727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 4, a(1) = 5, a(n) = a(n-1) + a(n-2) - 4 (n >= 2)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is Fibonacci(n) + 4 with 0‑based indexing.  The function
   memoises computed values so that any order of calls is efficient. */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cached_len = 0;   /* length of allocated array    */
static idx_t cached_max = 0;   /* highest index already computed */

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL)
    {
        cache = (seq_t *)realloc(NULL, 2 * sizeof(seq_t));
        if (cache == NULL)  /* allocation failure – return 0 as safe fallback */
            return 0;
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)5;
        cached_len = 2;
        cached_max = 1U;
    }

    /* small indices handled directly */
    if (n <= cached_max)
        return cache[n];

    /* grow cache to accommodate index n */
    if (n + 1 > cached_len)
    {
        size_t newlen = cached_len ? cached_len : 2;
        while (newlen <= (size_t)n + 1)
            newlen <<= 1;
        seq_t *newptr = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
        if (newptr == NULL)  /* allocation failure – return 0 as safe fallback */
            return 0;
        cache = newptr;
        cached_len = newlen;
    }

    /* compute new values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i)
    {
        cache[i] = cache[i - 1] + cache[i - 2] - (seq_t)4;
    }
    cached_max = n;
    return cache[n];
}
