/*
 * A080012  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={0}.
 * https://oeis.org/A080012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-2)+3*a(n-3)+4*a(n-4)+5*a(n-5)+a(n-6)
                  -2*a(n-7)-a(n-8)-a(n-9)-a(n-10)   for n ≥ 10
   Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=6, a(5)=11,
                   a(6)=23, a(7)=51, a(8)=113, a(9)=244 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* First ten terms */
static const seq_t base_vals[10] = {
    1ULL, 0ULL, 1ULL, 2ULL, 6ULL, 11ULL, 23ULL, 51ULL, 113ULL, 244ULL
};

/* Cache of computed values */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;   /* highest index stored + 1 */
static idx_t  cache_cap  = 0;   /* allocated capacity */

/* Ensure cache has space for indices up to idx (inclusive) */
static void ensure_cache(idx_t idx)
{
    if (idx + 1 <= cache_cap)
        return;
    idx_t new_cap = (cache_cap > 0) ? cache_cap * 2 : 10;
    if (new_cap < idx + 1)
        new_cap = idx + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* memory exhausted */
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Grow cache to at least n if not already done */
    if (n + 1 > cache_cap)
        ensure_cache(n);

    /* Fill base values if not yet done */
    if (!cache_size)
    {
        cache_size = 10;
        for (idx_t i = 0; i < 10; ++i)
            cache[i] = base_vals[i];
    }

    if (n < cache_size)
        return cache[n];

    /* Compute successive terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* Recurrence valid for i >= 10 */
        seq_t term = 0;
        term += ((seq_t)2) * cache[i-2];
        term += ((seq_t)3) * cache[i-3];
        term += ((seq_t)4) * cache[i-4];
        term += ((seq_t)5) * cache[i-5];
        term += cache[i-6];
        term -= ((seq_t)2) * cache[i-7];
        term -= cache[i-8];
        term -= cache[i-9];
        term -= cache[i-10];
        cache[i] = term;
    }

    cache_size = n + 1;
    return cache[n];
}
