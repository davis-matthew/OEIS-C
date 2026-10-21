/*
 * A229681  Number of defective 3-colorings of an n X 4 0..2 array connected
 *          diagonally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 27*a(n-1) - 243*a(n-2) + 729*a(n-3) for n > 6 * with base
 *            values a(1)=0, a(2)=360, a(3)=10896, * a(4)=186576,
 *            a(5)=2700432, a(6)=35485776
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;       /* dynamically sized cache for a(1)…a(n) */
static idx_t  cache_cap = 0;      /* allocated capacity (highest index + 1) */
static idx_t  cache_size = 0;     /* highest index for which a(i) is already stored */

/* Ensure we can write up to index 'needed' inclusive */
static void ensure_capacity(idx_t needed)
{
    if (needed < cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= needed)
        new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first six terms */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_capacity(6);
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)360;
    cache[3] = (seq_t)10896;
    cache[4] = (seq_t)186576;
    cache[5] = (seq_t)2700432;
    cache[6] = (seq_t)35485776;
    cache_size = 6;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, return 0 */
    init_cache();
    if (n <= cache_size)
        return cache[n];
    ensure_capacity(n);
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1] * (seq_t)27;
        seq_t t2 = cache[i - 2] * (seq_t)243;
        seq_t t3 = cache[i - 3] * (seq_t)729;
        cache[i] = t1 - t2 + t3;
    }
    cache_size = n;
    return cache[n];
}
