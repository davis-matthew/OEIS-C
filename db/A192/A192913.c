/*
 * A192913  Coefficient of x^2 in the reduction by (x^3 -> x + 1) of the
 *          polynomial F(n+1)*x^n, where F(n)=A000045 (Fibonacci sequence).
 * https://oeis.org/A192913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+4*a(n-2)+5*a(n-3)+2*a(n-4)-a(n-5)+a(n-6) with
 *            initial values a(0)=0, a(1)=0, a(2)=2, a(3)=3, a(4)=10, a(5)=32
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;            /* allocated size of the cache array */
static idx_t  cache_max = (idx_t)-1;    /* highest index that has been computed */

/* Initialise the cache with the first six terms */
static void cache_init(void)
{
    if (cache != NULL)
        return;

    cache_len = 6;
    cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
    if (!cache)
        abort();                       /* allocation failed */

    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)10;
    cache[5] = (seq_t)32;

    cache_max = 5;
}

/* Grow the cache array so that it can store at least indices up to needed */
static void cache_grow(idx_t needed)
{
    if (cache_len > 0 && cache_len > needed)
        return;

    size_t new_len = cache_len ? cache_len : 6;
    while ((idx_t)new_len <= needed)
        new_len <<= 1;                 /* double the size */

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failed */

    cache = tmp;
    cache_len = new_len;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n)
{
    cache_init();

    /* If the term has already been computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* Ensure the cache can hold the requested index */
    if (n >= cache_len)
        cache_grow(n);

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        seq_t t2 = 4 * cache[i-2];
        seq_t t3 = 5 * cache[i-3];
        seq_t t4 = 2 * cache[i-4];
        seq_t val = t1 + t2 + t3 + t4 - cache[i-5] + cache[i-6];
        cache[i] = val;
    }

    cache_max = n;
    return cache[n];
}
