/*
 * A179999  Length of the n-th term in the modified Look and Say sequence
 *          A110393.
 * https://oeis.org/A179999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-3) + a(n-4) - a(n-5) (for n > 6)
 *            Initial values: a(1)=1, a(2)=2, a(3)=2, a(4)=4, a(5)=6, a(6)=8
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 88, u64 n <= 180,
 *            u128 n <= 365, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* realloc, abort */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;      /* cache[i] holds a(i+1) (0‑based) */
static idx_t  cache_len  = 0;    /* allocated size of the cache   */
static idx_t  max_computed = 0;  /* largest n for which a(n) is known */

/* Ensure that the cache can hold at least 'needed' elements (0‑based) */
static void ensure_cache_capacity(idx_t needed)
{
    if (cache_len >= needed) return;
    size_t new_size = (size_t)needed * sizeof(seq_t);
    seq_t *tmp = (seq_t *)realloc(cache, new_size);
    if (tmp == NULL) abort();          /* out of memory */
    cache = tmp;
    cache_len = needed;
}

/* Initialise the first 6 entries of the cache */
static void init_cache(void)
{
    if (cache_len >= 6) return;        /* already initialised */

    ensure_cache_capacity(6);
    cache[0] = (seq_t)1;   /* a(1) */
    cache[1] = (seq_t)2;   /* a(2) */
    cache[2] = (seq_t)2;   /* a(3) */
    cache[3] = (seq_t)4;   /* a(4) */
    cache[4] = (seq_t)6;   /* a(5) */
    cache[5] = (seq_t)8;   /* a(6) */
    max_computed = 6;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* behaviour below offset is unspecified */

    init_cache();

    if (n > max_computed) {
        /* Grow the cache to accommodate up to n */
        ensure_cache_capacity(n);

        /* Compute the missing terms using the linear recurrence */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1)+a(i-2)-a(i-3)+a(i-4)-a(i-5) */
            /* indices in the cache are offset by –1 */
            seq_t term =
                cache[i-2] + cache[i-3]
                - cache[i-4] + cache[i-5]
                - cache[i-6];
            cache[i-1] = term;
        }
        max_computed = n;
    }

    return cache[n-1];
}
