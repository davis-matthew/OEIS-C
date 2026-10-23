/*
 * A028221  Expansion of 1/((1-7x)(1-8x)(1-9x)(1-12x)).
 * https://oeis.org/A028221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A028221
   Formula: a(n) = (12^(n+3) – 10·9^(n+3) + 15·8^(n+3) – 6·7^(n+3)) / 60
   Linear recurrence used for efficient computation:
   a(0)=1, a(1)=36, a(2)=817, a(3)=14964
   a(n)=36·a(n-1) – 479·a(n-2) + 2796·a(n-3) – 6048·a(n-4) for n≥4
   (Both formulas are mathematically identical.) */
#include <stddef.h>
#include "../utils/ensure_cache.h"
#include "../utils/mul_small.h"

/* Static cache – shared among calls */
static seq_t *cache = NULL;          /* array of a(i) */
static size_t cache_cap = 0;         /* allocated capacity */
static idx_t max_computed = 0;       /* highest index already computed */

/* Return a(n) for the sequence A028221 */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise base values once */
    if (max_computed < 3) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)36;
        cache[2] = (seq_t)817;
        cache[3] = (seq_t)14964;
        max_computed = 3;
    }

    /* If we already have the requested value, just return it */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t t1 = mul_small(cache[k - 1], 36);
        seq_t t2 = mul_small(cache[k - 2], 479);
        seq_t t3 = mul_small(cache[k - 3], 2796);
        seq_t t4 = mul_small(cache[k - 4], 6048);
        /* Summation and subtraction are safe: the first sum ≥ second sum */
        cache[k] = (t1 + t3) - (t2 + t4);
    }
    max_computed = n;
    return cache[n];
}
