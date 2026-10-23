/*
 * A022092  Fibonacci sequence beginning 0, 9.
 * https://oeis.org/A022092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * Fibonacci(n). Recurrence: a(0) = 0 a(1) = 9 a(n) =
 *            a(n-1) + a(n-2) for n >= 2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cached values of the sequence.  The cache covers all n
   that have been computed so far.  The array is grown
   geometrically to keep amortised O(1) work per call. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static idx_t cache_max = 0;    /* highest n for which cache[n] is valid */

/* Ensure the cache can hold entries up to index n inclusive */
static void ensure_cache_capacity(idx_t n)
{
    size_t required = (size_t)(n + 1);
    if (required <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < required)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* allocation failed – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first two values.  This is
   done lazily on the first call. */
static void init_cache(void)
{
    if (cache_cap == 0) {
        ensure_cache_capacity(1);   /* at least space for indices 0 and 1 */
        cache[0] = 0;
        cache[1] = 9;
        cache_max = 1;
    }
}

/* Return a(n) = 9*Fibonacci(n).  The value is computed by
   recurrence a(n)=a(n-1)+a(n-2).  Any order of calls
   is supported because the cache grows as needed. */
seq_t a(idx_t n)
{
    init_cache();                    /* makes sure cache[0] and cache[1] exist */

    /* Grow the cache up to n if necessary */
    if ((idx_t)cache_max < n) {
        ensure_cache_capacity(n);
        /* compute new terms on demand */
        for (idx_t i = cache_max + 1; i <= n; ++i)
            cache[i] = cache[i - 1] + cache[i - 2];
        cache_max = n;
    }

    return cache[n];
}
