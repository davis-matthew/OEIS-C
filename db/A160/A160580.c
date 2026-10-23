/*
 * A160580  Positive numbers y such that y^2 is of the form x^2+(x+457)^2 with
 *          integer x.
 * https://oeis.org/A160580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; Initial values: a(1)=325,
 *            a(2)=457, a(3)=877, a(4)=1073, a(5)=2285, a(6)=4937
 * Range:     u8 none, u16 n <= 10, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[0] unused, cache[i] = a(i) */
static idx_t       cache_high = 0;   /* highest index cached */

/* Initialise cache with the first six terms. */
static void init_cache(void)
{
    size_t sz = 7;   /* indices 0..6 */
    cache = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!cache) abort();
    for (size_t i = 0; i < sz; ++i) cache[i] = 0;
    cache[1] = 325; cache[2] = 457; cache[3] = 877;
    cache[4] = 1073; cache[5] = 2285; cache[6] = 4937;
    cache_high = 6;
}

/* Expand cache to cover at least index n, computing new terms if needed. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) init_cache();

    if (n <= cache_high) return;                 /* already computed */

    size_t new_sz = (size_t)n + 1;               /* indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (!new_cache) abort();

    /* zero the new entries that will be filled by the recurrence */
    for (idx_t i = cache_high + 1; i <= n; ++i) new_cache[i] = 0;
    cache = new_cache;

    /* compute the new terms of the recurrence */
    for (idx_t i = cache_high + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i - 3] - cache[i - 6];
    }
    cache_high = n;
}

/* Public function requested in the specification. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 – return 0 safely */

    if (n <= 6) {                 /* initial values are known */
        static const seq_t init[7] = {0, 325, 457, 877, 1073, 2285, 4937};
        return init[(size_t)n];
    }

    ensure_cache(n);
    return cache[(size_t)n];
}
