/*
 * A155698  a(n)=a(n-1)+a(n-2)^a(n-3); a(1)=a(2)=a(3)=1.
 * https://oeis.org/A155698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2)^a(n-3); Initial values: a(1)=a(2)=a(3)=1;
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute recurrence with memoisation */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static seq_t *cache = NULL;     /* array of computed terms, 1‑based */
static size_t cache_cap = 0;    /* allocated size of cache (bytes/1) */
static idx_t cache_size = 0;    /* largest n for which a(n) is computed */

/* Ensure cache contains values up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;                       /* already computed */

    /* Extend storage if needed */
    if (cache_cap < (size_t)(n + 1)) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = (size_t)(n + 1);
    }

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = 1;            /* base cases */
        } else {
            /* use big_pow to raise a(i-2) to the power a(i-3) */
            seq_t term = big_pow(cache[i - 2], cache[i - 3]);
            cache[i] = cache[i - 1] + term;
        }
    }
    cache_size = n;
}

/* Public API required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1)
        abort();                       /* undefined for n < 1 */
    ensure_cache(n);
    return cache[n];
}
