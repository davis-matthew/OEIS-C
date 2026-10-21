/*
 * A050536  Iterated triangular numbers with seed 8.
 * https://oeis.org/A050536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = binomial(a(n-1)+1, 2) with a(0) = 8 (OEIS A050536)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Memoization cache */
static seq_t *cache = NULL;     /* array of computed terms   */
static size_t cache_cap = 0;    /* current allocation size   */
static idx_t  last_computed = 0;/* highest n computed so far */

static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;

    size_t new_cap = cache_cap == 0 ? 1 : cache_cap;
    while (new_cap <= (size_t)n) {
        new_cap *= 2;
    }

    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;

    /* initialise first element if this is the first allocation */
    if (cache_cap >= 1 && last_computed == 0 && n >= 0) {
        cache[0] = (seq_t)8;
        last_computed = 0;
    }
}

seq_t a(idx_t n)
{
    /* Validate input: OEIS offset is 0, negative n not used */
    if (n == (idx_t)(-1)) {
        return 0;
    }

    ensure_cache(n);

    /* Return cached value if already computed */
    if (n <= last_computed) {
        return cache[(size_t)n];
    }

    /* Compute forward from last_computed+1 up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t x = prev + (seq_t)1;

        /* Compute binomial(x,2) without overflow where possible */
        if ((x & (seq_t)1) == 0) {
            cache[(size_t)i] = (x / (seq_t)2) * (x - (seq_t)1);
        } else {
            cache[(size_t)i] = x * ((x - (seq_t)1) / (seq_t)2);
        }
    }

    last_computed = n;
    return cache[(size_t)n];
}
