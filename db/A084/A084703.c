/*
 * A084703  Squares k such that 2*k+1 is also a square.
 * https://oeis.org/A084703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A084703 – squares k such that 2*k+1 is also a square
   Formula: a(n) = ((17+12*sqrt(2))^n + (17-12*sqrt(2))^n - 2)/8
   Recurrence: a(0)=0, a(1)=4; a(n)=34*a(n-1)-a(n-2)+8   for n>=2   */
/* The implementation uses a simple dynamic cache and the above recurrence.  */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;
static idx_t   cache_len = 0;   /* number of terms actually stored   */
static idx_t   cache_cap = 0;   /* allocated capacity                 */

static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;          /* already cached */

    /* Expand capacity until it can hold indices 0..n */
    if (cache_cap == 0) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        /* initialise first two terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }
    while (cache_cap <= n) {
        cache_cap <<= 1;                 /* double */
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) abort();
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i)=34*a(i-1)-a(i-2)+8 */
        seq_t term = cache[i-1] * (seq_t)34 - cache[i-2] + (seq_t)8;
        cache[i] = term;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
