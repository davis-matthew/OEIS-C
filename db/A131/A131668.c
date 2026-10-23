/*
 * A131668  Smallest number whose sum of digits is 2n+1.
 * https://oeis.org/A131668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + 100*a(n-9) - 100*a(n-10) * with initial values:
 *            a(0)=1, a(1)=3, a(2)=5, a(3)=7, a(4)=9, * a(5)=29, a(6)=49,
 *            a(7)=69, a(8)=89, a(9)=199. * The recurrence holds for all n ≥
 *            10
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache of computed terms */
static seq_t *cache = NULL;        /* array of computed values */
static size_t cache_len = 0;       /* number of terms actually stored */
static size_t cache_alloc = 0;     /* capacity of the array */

/* Ensure that cache can hold at least (needed+1) elements. */
static void ensure_cache(idx_t needed)
{
    size_t required = (size_t)needed + 1;
    if (required <= cache_alloc)
        return;

    size_t new_alloc = cache_alloc ? cache_alloc * 2 : 10;
    if (new_alloc < required)
        new_alloc = required;

    /* cast to seq_t* is required for C++ compatibility */
    seq_t *new_cache = (seq_t *)malloc(new_alloc * sizeof(seq_t));
    if (!new_cache)
        exit(1);   /* allocation failure */

    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache)
        free(cache);

    cache = new_cache;
    cache_alloc = new_alloc;
}

/* The sequence A131668 */
seq_t a(idx_t n)
{
    if (n < cache_len)
        return cache[(size_t)n];

    ensure_cache(n);

    /* initial values for n = 0..9 */
    static const seq_t initial[10] =
        { 1, 3, 5, 7, 9, 29, 49, 69, 89, 199 };

    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i < 10)
        {
            cache[(size_t)i] = initial[(size_t)i];
        }
        else
        {
            seq_t v   = cache[(size_t)(i - 1)];
            v        += 100 * cache[(size_t)(i - 9)];
            v        -= 100 * cache[(size_t)(i - 10)];
            cache[(size_t)i] = v;
        }
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
