/*
 * A156748  a(1) = a(2) = a(3) = a(4) = 0, then a(n) = abs(a(n-1) -a(n-2)
 *          +2*a(n-3) -a(n-4)) -a(n-3) -1.
 * https://oeis.org/A156748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = abs(a(n-1) - a(n-2) + 2*a(n-3) - a(n-4)) - a(n-3) - 1
 *            with initial values a(1)=a(2)=a(3)=a(4)=0 and offset 1
 * Range:     i8 n <= 204, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached sequence values.  cache[0] corresponds to a(1). */
static seq_t   *cache   = NULL;
static size_t   cached_len = 0;   /* number of terms currently stored */
static size_t   cache_cap  = 0;   /* allocated array size */

/* Ensure the cache has enough storage for at least `need` terms. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap ? cache_cap : 16;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp)  /* allocation failed; abort */
    {
        free(cache);
        cache = NULL;
        cached_len = 0;
        cache_cap  = 0;
        return;
    }
    cache = tmp;
    cache_cap = newcap;
}

/* Compute a(n) for n ≥ 1.  The function is memoised but works for calls
   in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1 */

    /* Initialise the first four terms if the cache is empty. */
    if (cached_len < 4)
    {
        size_t need = 4;
        ensure_cache_cap(need);
        if (!cache)   /* allocation failed */
            return 0;
        cache[0] = cache[1] = cache[2] = cache[3] = 0;
        cached_len = 4;
    }

    if (n <= cached_len)
        return cache[n - 1];

    /* Compute missing terms up to n.  Preserve the original length to
       know from which index the loop should start. */
    size_t old_len = cached_len;
    ensure_cache_cap((size_t)n);
    for (idx_t i = old_len + 1; i <= n; ++i)
    {
        /* Indices:
           a(i-1) -> cache[i-2]
           a(i-2) -> cache[i-3]
           a(i-3) -> cache[i-4]
           a(i-4) -> cache[i-5]  (valid because i ≥ 5) */
        seq_t a1 = cache[i - 2];
        seq_t a2 = cache[i - 3];
        seq_t a3 = cache[i - 4];
        seq_t a4 = cache[i - 5];
        seq_t v  = a1 - a2 + 2 * a3 - a4;
        seq_t absv = v >= 0 ? v : -v;
        seq_t res  = absv - a3 - 1;
        cache[i - 1] = res;
    }
    cached_len = (size_t)n;
    return cache[n - 1];
}
