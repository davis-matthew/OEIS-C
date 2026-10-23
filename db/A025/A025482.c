/*
 * A025482  a(0) = 0; a(n) = a(n/3)/3 if n = a(n/3) = 0 (mod 3); a(n) =
 *          a(n-1)+1 otherwise.
 * https://oeis.org/A025482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = a(n/3)/3 if (n % 3 == 0 && a(n/3) % 3 == 0);
 *            a(n) = a(n-1) + 1 otherwise
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses memoisation to compute each term at most once.
   It allocates a dynamic array of seq_t to store the values.  */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation table */
static idx_t cache_len = 0;     /* number of values currently stored */

/* Returns the n‑th term of the sequence A025482.  */
seq_t a(idx_t n)
{
    /* Grow the cache if necessary.  */
    if (n >= cache_len)
    {
        size_t new_len = (size_t)n + 1;               /* inclusive */
        seq_t *p;

        /* The cast is needed for C++ to convert void* to seq_t*.
           In C this cast is optional, but it keeps the code valid
           for both languages. */
        p = (cache_len == 0)
            ? (seq_t*)malloc(new_len * sizeof(seq_t))
            : (seq_t*)realloc(cache,  new_len * sizeof(seq_t));

        if (!p)               /* allocation failed – keep the old cache */
            return 0;

        cache = p;

        /* Compute the new entries from the old length up to n.  */
        for (size_t i = cache_len; i <= n; ++i)
        {
            if (i == 0)
            {
                cache[0] = 0;                        /* base case */
            }
            else
            {
                /* i is a size_t; the conditions reference i and
                   the previously computed value cache[i/3].  */
                if ((i % 3u == 0u) &&
                    ((cache[i / 3] % (seq_t)3u) == 0u))
                {
                    cache[i] = cache[i / 3] / (seq_t)3u;
                }
                else
                {
                    cache[i] = cache[i - 1] + (seq_t)1u;
                }
            }
        }

        cache_len = new_len;
    }

    return cache[n];
}
