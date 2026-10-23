/*
 * A179211  Number of squarefree numbers between n and 2*n (inclusive).
 * https://oeis.org/A179211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=n..2*n} A008966(k) (count of square‑free integers)
 * Range:     u8 n <= 414, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached boolean array: squarefree_cache[i] == 1  ⇔  i is square‑free */
static char *squarefree_cache = NULL;
static idx_t cache_limit = 0;   /* highest index currently cached (inclusive) */

/* Build a square‑free sieve up to ``limit`` and replace the existing cache. */
static void build_squarefree_cache(idx_t limit)
{
    /* Allocate new cache.  Cast required for C++ compilation. */
    char *new_cache = (char *)malloc((size_t)(limit + 1));
    if (!new_cache) abort();          /* fatal if memory cannot be allocated */

    /* Initially assume every number is square‑free. */
    for (idx_t i = 0; i <= limit; ++i)
        new_cache[i] = 1;

    /* Mark multiples of prime squares as non–square‑free. */
    idx_t max_p = 2;
    while (max_p * max_p <= limit) ++max_p;
    --max_p;
    for (idx_t p = 2; p <= max_p; ++p) {
        if (new_cache[p] == 0) continue;   /* skip numbers already known to have a square factor */
        idx_t sq = p * p;
        for (idx_t j = sq; j <= limit; j += sq)
            new_cache[j] = 0;
    }

    /* Replace the old cache. */
    free(squarefree_cache);
    squarefree_cache = new_cache;
    cache_limit = limit;
}

/* Return the number of square‑free integers in [n, 2*n]. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* behaviour below the offset is irrelevant */

    idx_t limit = n << 1;              /* 2 * n, safe for the expected range */

    /* Ensure the cache covers the interval [0, limit]. */
    if (cache_limit < limit)
        build_squarefree_cache(limit);

    /* Count square‑free numbers in the requested range. */
    seq_t count = 0;
    for (idx_t k = n; k <= limit; ++k)
        if (squarefree_cache[k])
            ++count;

    return count;
}
