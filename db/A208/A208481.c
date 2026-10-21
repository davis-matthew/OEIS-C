/*
 * A208481  Diagonal sums of triangle A185384.
 * https://oeis.org/A208481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + a(n-3) + a(n-4) a(0) = 1, a(1) = 2, a(2) = 6,
 *            a(3) = 19
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for already computed values */
static seq_t *cache = NULL;      /* array of seq_t values */
static size_t cap = 0;           /* current allocated capacity (number of slots) */
static idx_t last_calc = (idx_t)-1; /* highest index that has been computed; sentinel is 0xFFFFFFFF... */

/* ensure that cache has space for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (cap >= needed) return;
    size_t new_cap = cap > 0 ? cap : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap = new_cap;
}

/* sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    /* initialize base terms on first call */
    if (last_calc == (idx_t)-1)
    {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)19;
        last_calc = 3;
    }
    /* if already computed, return directly */
    if (n <= last_calc) return cache[n];
    /* compute missing values up to n */
    for (idx_t i = last_calc + 1; i <= n; ++i)
    {
        cache[i] = cache[i-1] * 3 + cache[i-3] + cache[i-4];
    }
    last_calc = n;
    return cache[n];
}
