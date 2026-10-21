/*
 * A098262  First differences of Chebyshev polynomials S(n,627) = A098260(n)
 *          with Diophantine property.
 * https://oeis.org/A098262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 627*a(n-1) - a(n-2), a(0)=1, a(1)=626
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* dynamic array of terms          */
static idx_t  cache_size = 0;     /* number of terms stored          */
static idx_t  cache_cap  = 0;     /* allocated capacity in elements   */

/* Grow cache to at least 'need' elements.  Capacity always stays a power of 2. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory – terminate the program safely. */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A098262.  Calls are memoised so that
   arbitrary ordering of n remains fast. */
seq_t a(idx_t n)
{
    /* Initialise the first two terms the first time the function is used. */
    if (cache_size == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)626;
        cache_size = 2;
    }

    /* If the requested term is already in the cache, return it. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache up to n. */
    ensure_capacity(n + 1);
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Use the recurrence: a(i) = 627*a(i-1) - a(i-2). */
        cache[i] = (seq_t)(627 * cache[i - 1] - cache[i - 2]);
    }

    cache_size = n + 1;
    return cache[n];
}
