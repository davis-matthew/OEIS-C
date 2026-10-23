/*
 * A039721  a(1) = 1, a(m+1) = 2*Sum_{k=1..floor((m+1)/2)} a(m+1-k).
 * https://oeis.org/A039721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1, a(2)=2, a(2m+1)=3*a(2m)-2*a(m), a(2m+2)=3*a(2m+1) */
#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;    /* highest allocated index plus one */
static idx_t max_computed = 0; /* largest n for which cache[n] is defined */

/* Ensure that cache has room for indices 0 .. n inclusive */
static void ensure_cache(idx_t n)
{
    idx_t required = n + 1;            /* we keep index 0 unused */
    if (required <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 32;
    while (new_cap < required)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for offset 1, but safe fallback */

    ensure_cache(n);

    if (n <= max_computed)
        return cache[n];

    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[1] = (seq_t)1;
        else if (i == 2)
            cache[2] = (seq_t)2;
        else if ((i & 1) == 0) /* even: i = 2m+2 */
            cache[i] = cache[i - 1] * (seq_t)3;
        else
            cache[i] = cache[i - 1] * (seq_t)3 - cache[(i - 1) / 2] * (seq_t)2;
    }

    max_computed = n;
    return cache[n];
}
