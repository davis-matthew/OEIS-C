/*
 * A021924  Expansion of 1/((1-x)(1-4x)(1-8x)(1-11x)).
 * https://oeis.org/A021924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (14*11^(n+3) - 35*8^(n+3) + 35*4^(n+3) - 14)/2940 *
 *            Recurrence: a(0)=1, a(1)=24, a(2)=389, a(3)=5364, *
 *            a(n)=24*a(n-1)-187*a(n-2)+516*a(n-3)-352*a(n-4) (n≥4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoized values */
static seq_t *a_cache = NULL;
static idx_t   a_cache_cap = 0;
static idx_t   a_cache_len = 0;

/* Ensure the cache holds at least indices 0 … n */
static void a_ensure_cache(idx_t n)
{
    if (n + 1 <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
    while (new_cap <= (size_t)(n + 1))
        new_cap <<= 1;
    /* realloc returns void* in C++, cast explicitly */
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;
    a_cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise cache with first four terms if empty */
    if (a_cache_len == 0)
    {
        a_ensure_cache(3);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)24;
        a_cache[2] = (seq_t)389;
        a_cache[3] = (seq_t)5364;
        a_cache_len = 4;
    }

    /* Return already stored value */
    if (n < a_cache_len)
        return a_cache[(size_t)n];

    /* Ensure enough space for needed terms */
    a_ensure_cache(n);
    for (idx_t i = a_cache_len; i <= n; ++i)
    {
        seq_t term = (seq_t)24   * a_cache[(size_t)(i-1)];
        term += (seq_t)516   * a_cache[(size_t)(i-3)];
        term -= (seq_t)187   * a_cache[(size_t)(i-2)];
        term -= (seq_t)352   * a_cache[(size_t)(i-4)];
        a_cache[(size_t)i] = term;
    }
    a_cache_len = n + 1;
    return a_cache[(size_t)n];
}
