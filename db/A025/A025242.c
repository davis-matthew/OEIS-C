/*
 * A025242  Generalized Catalan numbers A(x)^2 -(1+x)^2*A(x) +x*(2+x+x^2) =0.
 * https://oeis.org/A025242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1}^{n-3} a(i)*a(n-i) for n ≥ 4; a(1) = 2, a(2) = 1,
 *            a(3) = 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 23, u64 n <= 42, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Cache of computed terms.  1‑based indexing; cache[0] unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can store indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    /* Convert n to size_t for array bounds.  n is unsigned so this is safe
       for the intended range (up to about 10 000). */
    size_t need = (size_t)n + 1;          /* we use one extra slot for 1‑based array */
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap : 16;
    while (new_cap < need)
        new_cap <<= 1;                    /* double until enough */

    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* unrecoverable memory failure */

    /* Copy existing data */
    if (cache) {
        memcpy(tmp, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }

    /* Zero‑initialize the new part */
    memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));

    cache = tmp;
    cache_cap = new_cap;
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for n=0 */

    ensure_capacity(n);

    /* Initialise the three known values if not yet set. */
    if (cache[1] == 0) cache[1] = (seq_t)2;
    if (cache[2] == 0) cache[2] = (seq_t)1;
    if (cache[3] == 0) cache[3] = (seq_t)1;

    /* Compute new terms up to n, using memoisation. */
    for (idx_t k = 4; k <= n; ++k) {
        if (cache[(size_t)k] != 0)          /* already computed */
            continue;

        seq_t sum = 0;
        for (idx_t i = 1; i <= k - 3; ++i) {
            seq_t prod = cache[(size_t)i] * cache[(size_t)(k - i)];
            sum += prod;
        }
        cache[(size_t)k] = sum;
    }

    return cache[(size_t)n];
}
