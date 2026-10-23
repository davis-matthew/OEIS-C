/*
 * A038201  5-wave sequence.
 * https://oeis.org/A038201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1)+a(n-2) if n = 4*m+1, * a(n) = a(n-1)+a(n-4) if n =
 *            4*m+2, * a(n) = a(n-1)+a(n-6) if n = 4*m+3, * a(n) =
 *            a(n-1)+a(n-8) if n = 4*m. * Initial values:
 *            a(0)=a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoised values, indexed by n */
static size_t cache_len = 0;     /* current allocated length of cache */
static idx_t last_computed = 4;  /* highest n for which cache is valid */

/* Ensure the cache is large enough to hold index n */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_len) return;

    size_t newlen = (size_t)n + 1;
    if (newlen < 16) newlen = 16;

    /* C-style cast works in both C and C++ */
    seq_t *tmp = (seq_t *)realloc(cache, newlen * sizeof(seq_t));
    cache = tmp;
    cache_len = newlen;
}

seq_t a(idx_t n)
{
    /* Initialise cache with first 5 terms on first call */
    if (!cache)
    {
        cache_len = 16;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 1;
        last_computed = 4;
    }

    if (n <= last_computed)
        return cache[n];

    ensure_cache_size(n);

    /* Compute terms from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        if (i % 4 == 1)          /* i = 4*m+1 */
            cache[i] = cache[i-1] + cache[i-2];
        else if (i % 4 == 2)     /* i = 4*m+2 */
            cache[i] = cache[i-1] + cache[i-4];
        else if (i % 4 == 3)     /* i = 4*m+3 */
            cache[i] = cache[i-1] + cache[i-6];
        else                     /* i % 4 == 0 => i = 4*m   */
            cache[i] = cache[i-1] + cache[i-8];
    }

    last_computed = n;
    return cache[n];
}
