/*
 * A207276  Number of n X 1 0..3 arrays avoiding the patterns z z+1 z or z z-1
 *          z in any row, column, diagonal or antidiagonal.
 * https://oeis.org/A207276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) + a(n-3) + 3*a(n-4) + a(n-5); a(1)=4,
 *            a(2)=16, a(3)=58, a(4)=214, a(5)=788
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values of the sequence.  The array is indexed 0‑based
   where cache[0] = a(1).  We grow it as needed. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* allocated capacity */
static size_t cache_len  = 0;   /* number of stored terms */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cache_size)
        return;
    size_t new_size = cache_size ? cache_size * 2 : 16;
    if (new_size < need)
        new_size = need;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1 */

    /* Initialise the cache once. */
    if (cache_len == 0) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)                      /* allocation failed */
            return (seq_t)0;
        /* initial terms */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)58;
        cache[3] = (seq_t)214;
        cache[4] = (seq_t)788;
        cache_len = 5;
    }

    /* Return stored value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Extend the cache up to n. */
    size_t target = (size_t)n;
    ensure_capacity(target);
    for (size_t i = cache_len; i < target; ++i) {
        /*   a(i+1) = 3*a(i) + 2*a(i-1) + a(i-2) + 3*a(i-3) + a(i-4)   */
        cache[i] = 3 * cache[i - 1]
                 + 2 * cache[i - 2]
                 +       cache[i - 3]
                 + 3 * cache[i - 4]
                 +       cache[i - 5];
    }
    cache_len = target;
    return cache[(size_t)n - 1];
}
