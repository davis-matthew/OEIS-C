/*
 * A063057  'Reverse and Add!' trajectory of 7059.
 * https://oeis.org/A063057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(0) = 7059, a(n) = a(n-1) + reverse(a(n-1)) for n ≥ 1
 * Range:     u8 none, u16 n <= 1, u32 n <= 12, u64 n <= 34, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t reverse_digits(seq_t x)
{
    seq_t rev = (seq_t)0;
    while (x > (seq_t)0)
    {
        seq_t digit = x % (seq_t)10;
        rev = rev * (seq_t)10 + digit;
        x = x / (seq_t)10;
    }
    return rev;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array of computed terms */
    static size_t cache_len = 0;     /* number of terms currently stored (cache_len = last_index + 1) */
    static size_t cache_size = 0;    /* allocated capacity of the array */

    /* allocate or grow the cache if needed */
    if ((size_t)n + 1 > cache_size)
    {
        size_t new_size = cache_size ? cache_size : 64;
        while ((size_t)n + 1 > new_size)
            new_size <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = new_cache;
        cache_size = new_size;
    }

    /* initialise the first term if this is the first call */
    if (cache_len == 0)
    {
        cache[0] = (seq_t)7059;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        seq_t rev = reverse_digits(cache[i - 1]);
        cache[i] = cache[i - 1] + rev;
    }
    cache_len = (size_t)n + 1;

    return cache[n];
}
