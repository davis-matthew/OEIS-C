/*
 * A033713  Number of zeros in numbers 1 to 999..9 (n digits).
 * https://oeis.org/A033713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, easy
 * Formula:   a(n) = Sum_{i=1..n} 9*i*10^(i-1) Equivalent linear recurrence:
 *            a(1)=0, a(2)=9, a(3)=189 a(n)=21*a(n-1) - 120*a(n-2) +
 *            100*a(n-3) for n>=4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_n = 0;    /* highest index computed in cache */
static idx_t  cache_size = 0; /* allocated array size */

/* Ensure cache is large enough and compute missing values */
static void ensure_cache(idx_t n)
{
    if (n <= cache_n)
        return;

    /* Grow array if necessary */
    if (cache == NULL) {
        cache_size = n + 1;
        cache      = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE); /* abort on allocation failure */
    } else if ((idx_t)(n + 1) > cache_size) {
        cache_size = n + 1;
        cache      = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Compute from the first missing index up to n */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        if (i == 1) { cache[1] = 0; }
        else if (i == 2) { cache[2] = 9; }
        else if (i == 3) { cache[3] = 189; }
        else {
            /* recurrence: a(i) = 21*a(i-1) - 120*a(i-2) + 100*a(i-3) */
            seq_t term1 = ((seq_t)21) * cache[i - 1];
            seq_t term2 = ((seq_t)120) * cache[i - 2];
            seq_t term3 = ((seq_t)100) * cache[i - 3];
            cache[i] = term1 - term2 + term3;
        }
    }
    cache_n = n;
}

seq_t a(idx_t n)
{
    /* OEIS offset starts at 1; undefined for n==0 but harmless */
    ensure_cache(n);
    return cache[n];
}
