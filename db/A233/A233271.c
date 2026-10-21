/*
 * A233271  a(0)=0; thereafter a(n+1) = a(n) + 1 + number of 0's in binary
 *          representation of a(n), counted with A080791.
 * https://oeis.org/A233271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 8727, u32 n <= 8727, u64 n <= 8727,
 *            u128 n <= 8727, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(0) = 0; for n > 0,
 * a(n) = a(n-1) + 1 + (bitlen(a(n-1)) – popcount(a(n-1)))
 * where bitlen(x) is the number of bits needed to represent x
 * (0 for x = 0) and popcount(x) is the number of 1‑bits. */

/* Count 1-bits in a value of type seq_t. */
static unsigned popcount_seq_t(seq_t x)
{
    unsigned c = 0;
    while (x)
    {
        c += (unsigned)(x & 1);
        x >>= 1;
    }
    return c;
}

/*
 * Persistent cache: cache[i] holds a(i).  The cache grows on demand
 * and is independent of the order of calls, satisfying the required
 * memoisation behaviour.
 */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* allocated size */
static idx_t cache_max = 0;   /* largest index fully computed */

/* Ensure the cache contains at least indices 0..n inclusive */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max)
        return;                /* already computed */

    /* Grow allocation if necessary */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        if (cache_cap == 0)
            new_cap = 1;
        else if (new_cap < cache_cap * 2)
            new_cap = cache_cap * 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute new values */
    for (idx_t i = cache_max + 1; i <= n; ++i)
    {
        seq_t prev = cache[i - 1];
        seq_t zeros = 0;
        seq_t t = prev;
        while (t)
        {
            if ((t & 1) == 0)
                zeros++;
            t >>= 1;
        }
        cache[i] = prev + 1 + zeros;
    }
    cache_max = n;
}

/* Public constructor for the sequence */
seq_t a(idx_t n)
{
    /* Lazy initialisation of the cache */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 0;
        cache_cap = 1;
        cache_max = 0;
    }

    ensure_cache(n);

    return cache[n];
}
