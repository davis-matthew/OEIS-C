/*
 * A192861  Flat numbers: odd n such that n+1 is a squarefree number times a
 *          power of two.
 * https://oeis.org/A192861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Flat numbers: odd n such that n+1 is a squarefree number times a power of two.
   The sequence is defined by that condition; this implementation
   generates the numbers in increasing order and memoizes them. */
#include <stdbool.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* stored flat numbers */
static size_t cache_size = 0;        /* number of elements in cache */
static size_t cache_cap   = 0;        /* allocated capacity */

static bool
_squarefree(seq_t x)
{
    /* x is odd and expected to be squarefree if this function returns true */
    for (seq_t p = 2; p * p <= x; ++p)
    {
        seq_t pp = p * p;
        if (x % pp == 0)
            return false;
    }
    return true;
}

static bool
_is_flat(seq_t n)
{
    /* n must be odd */
    seq_t x = n + 1;          /* even number */
    seq_t m = x;
    while ((m & 1) == 0)      /* remove all factors of two */
        m >>= 1;
    return _squarefree(m);
}

static void
_ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1-based index; throw away values for n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* generate enough values to reach index n */
    if (cache_size < (size_t)n)
    {
        _ensure_capacity((size_t)n);

        /* start from 1 if the cache is empty, otherwise from the
           next odd number after the last cached value */
        seq_t cand = (cache_size == 0) ? 1 : (cache[cache_size - 1] + 2);
        while (cache_size < (size_t)n)
        {
            if (_is_flat(cand))
            {
                cache[cache_size++] = cand;
            }
            cand += 2;          /* only odd candidates */
        }
    }
    return cache[n - 1];
}
