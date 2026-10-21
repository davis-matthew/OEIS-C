/*
 * A072878  a(n) = 4*a(n-1)*a(n-2)*a(n-3) - a(n-4) with a(1) = a(2) = a(3) =
 *          a(4) = 1.
 * https://oeis.org/A072878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1)*a(n-2)*a(n-3) - a(n-4), with
 *            a(1)=a(2)=a(3)=a(4)=1
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static idx_t cache_len = 0;   /* number of terms currently stored */
    static seq_t *cache    = NULL;/* array of terms, 0‑based */

    /* Initialise cache with the first four terms on first call */
    if (cache_len == 0)
    {
        cache_len = 4;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;  /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)1; /* a(1) */
        cache[1] = (seq_t)1; /* a(2) */
        cache[2] = (seq_t)1; /* a(3) */
        cache[3] = (seq_t)1; /* a(4) */
    }

    /* Return cached value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache to accommodate term n */
    idx_t new_len = n;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) return (seq_t)0; /* allocation failure – unlikely */
    cache = new_cache;

    /* Compute terms from cache_len+1 up to n */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        /* indices: i-1 -> a(i) in cache (0‑based) */
        seq_t a1 = cache[i - 2]; /* a(i-1) */
        seq_t a2 = cache[i - 3]; /* a(i-2) */
        seq_t a3 = cache[i - 4]; /* a(i-3) */
        seq_t a4 = cache[i - 5]; /* a(i-4) */

        seq_t prod = a1 * a2; /* a(i-1) * a(i-2) */
        prod *= a3;           /* * a(i-3) */
        prod *= (seq_t)4;     /* * 4 */

        cache[i - 1] = prod - a4; /* a(i) = 4*a(i-1)*a(i-2)*a(i-3) - a(i-4) */
    }

    cache_len = new_len;
    return cache[n - 1];
}
