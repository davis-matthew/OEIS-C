/*
 * A173650  Expansion of x^2*(1 + 2*x - x^2) / ((1 + x)*(1 - x - 4*x^2 +
 *          2*x^3)).
 * https://oeis.org/A173650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) + 2*a(n-3) - 2*a(n-4) for n > 4 with initial
 *            values: a(0)=0, a(1)=0, a(2)=1, a(3)=2, a(4)=4 This implements
 *            the OEIS sequence A173650 via memoised linear recurrence
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;            /* allocated array of computed values */
static size_t cache_size = 0;           /* current allocated size of cache */
static idx_t max_computed = (idx_t)-1;  /* highest index currently computed (-1 if none) */

/* Ensure the cache holds at least 'need' elements, extending it if necessary.
   The cache is initialised with the first five terms. */
static void ensure_cache_capacity(idx_t need)
{
    if (need < 5) need = 5;                       /* need at least space for 0..4 */

    if (cache == NULL) {                          /* first time: allocate */
        cache_size = need;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return;                /* memory failure: will be handled by caller */

        /* initial terms */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;

        max_computed = 4;
        return;
    }

    if ((size_t)need <= cache_size) return;       /* already big enough */

    /* resize cache */
    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (new_cache == NULL) return;                /* memory failure: keep old cache */

    cache = new_cache;
    cache_size = (size_t)need;
}

/* Main function to compute a(n). */
seq_t a(idx_t n)
{
    /* Ensure we have a cache that holds at least n+1 elements. */
    ensure_cache_capacity(n + 1);
    if (cache == NULL) return (seq_t)0;           /* memory allocation failed */

    /* If we already have a(n), just return it. */
    if (n <= max_computed) return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term1 = (seq_t)5 * cache[i - 2];
        seq_t term2 = (seq_t)2 * cache[i - 3];
        seq_t term3 = (seq_t)2 * cache[i - 4];
        cache[i] = term1 + term2 - term3;          /* arithmetic fits in seq_t */
    }
    max_computed = n;
    return cache[n];
}
