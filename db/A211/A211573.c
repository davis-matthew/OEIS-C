/*
 * A211573  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having four
 *          or six distinct values for every i,j,k<=n.
 * https://oeis.org/A211573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 24*a(n-3) + 4*a(n-4) + 36*a(n-5) -
 *            24*a(n-6), for n ≥ 7 Initial terms: a(1)=16, a(2)=44, a(3)=108,
 *            a(4)=260, a(5)=604, a(6)=1436
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated capacity (elements) */
static idx_t computed_upto = 0; /* highest n computed; 0 means none */

/* Ensure the cache has space for at least n terms (1‑based) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cache_size)
        return;
    size_t new_size = (cache_size > 0) ? cache_size : 16;
    while (new_size < (size_t)n)
        new_size *= 2;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* behaviour for n<1 is arbitrary */

    /* Return cached value if already computed */
    if (n <= computed_upto)
        return cache[(size_t)n - 1];

    /* Extend cache to accommodate up to n */
    ensure_cache(n);

    /* Compute missing terms incrementally */
    for (idx_t i = computed_upto + 1; i <= n; ++i)
    {
        size_t idx = (size_t)i - 1; /* 0‑based index in cache */

        switch ((int)i)
        {
            case 1: cache[idx] = (seq_t)16; break;
            case 2: cache[idx] = (seq_t)44; break;
            case 3: cache[idx] = (seq_t)108; break;
            case 4: cache[idx] = (seq_t)260; break;
            case 5: cache[idx] = (seq_t)604; break;
            case 6: cache[idx] = (seq_t)1436; break;
            default:
            {
                seq_t t1 = cache[idx - 1]; /* a(i-1) */
                seq_t t2 = cache[idx - 2]; /* a(i-2) */
                seq_t t3 = cache[idx - 3]; /* a(i-3) */
                seq_t t4 = cache[idx - 4]; /* a(i-4) */
                seq_t t5 = cache[idx - 5]; /* a(i-5) */
                seq_t t6 = cache[idx - 6]; /* a(i-6) */

                /* Compute using the recurrence */
                cache[idx] = 3 * t1 + 6 * t2 - 24 * t3 + 4 * t4 + 36 * t5 - 24 * t6;
            }
        }
    }

    computed_upto = n;
    return cache[(size_t)n - 1];
}
