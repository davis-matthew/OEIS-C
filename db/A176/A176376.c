/*
 * A176376  y-values in the solution to x^2-69*y^2=1.
 * https://oeis.org/A176376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15550*a(n-1) - a(n-2), a(1)=0, a(2)=936
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;    /* highest index computed (1‑based) */

/* Ensure cache contains values up to n; compute missing terms */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Grow the array */
    idx_t new_cap = (n > 8) ? n : 8;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));

    /* Initialise the first two terms if not already done */
    if (cache_len < 1) { cache[0] = (seq_t)0; cache_len = 1; }
    if (cache_len < 2) { cache[1] = (seq_t)936; cache_len = 2; }

    /* Compute remaining terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t prev  = cache[i - 2];   /* a(i-1) */
        seq_t prevp = cache[i - 3];   /* a(i-2) */
        cache[i - 1] = (seq_t)15550 * prev - prevp;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    ensure_cache(n);
    return cache[n - 1];
}
