/*
 * A177260  Number of derangements of {1,2,...,n} having no adjacent 4-cycles
 *          (an adjacent 4-cycle is a cycle of the form (i,i+1,i+2,i+3)).
 * https://oeis.org/A177260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n‑1)*a(n‑1) + (n‑1)*a(n‑2) + 2*a(n‑4) + (n‑1)*a(n‑5) +
 *            3*a(n‑8) with a(0)=1, a(1)=0, a(2)=1, a(3)=2, a(4)=8, a(5)=44.
 *            For n<0 a(n)=0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for memoisation */
static seq_t *cache = NULL;          /* array of computed values */
static size_t cache_len = 0;         /* allocated array length */
static idx_t  last_computed = 0;     /* last index that has been filled */

/* initialise cache: allocate 9 positions and set the first six values */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* allocate at least 9 elements to cover 0..8 without further resizing */
    cache_len = 9;
    cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
    if (cache == NULL)
        return;   /* allocation failure – nothing else can be done */

    /* known initial values */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)2;
    cache[4] = (seq_t)8;
    cache[5] = (seq_t)44;
    /* remaining entries are already zero from calloc */
    last_computed = 5;
}

/* enlarge cache to be able to hold values up to index n */
static void ensure_cache_len(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cache_len >= needed)
        return;

    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (tmp == NULL)
        return;   /* realloc failed – nothing else can be done */

    /* zero the newly allocated portion */
    memset(tmp + cache_len, 0, (needed - cache_len) * sizeof(seq_t));

    cache = tmp;
    cache_len = needed;
}

/* compute a(n) using the recurrence, memoising results */
seq_t a(idx_t n)
{
    init_cache();

    /* if already computed, return cached value */
    if (n < cache_len && n <= last_computed)
        return cache[n];

    /* make sure the array is large enough */
    ensure_cache_len(n);

    /* compute all missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        seq_t term1 = 0, term2 = 0, term3 = 0, term4 = 0, term5 = 0;

        if (i >= 1)
            term1 = (seq_t)(i - 1) * cache[i - 1];

        if (i >= 2)
            term2 = (seq_t)(i - 1) * cache[i - 2];

        if (i >= 4)
            term3 = (seq_t)2 * cache[i - 4];

        if (i >= 5)
            term4 = (seq_t)(i - 1) * cache[i - 5];

        if (i >= 8)
            term5 = (seq_t)3 * cache[i - 8];

        cache[i] = term1 + term2 + term3 + term4 + term5;
    }

    last_computed = n;
    return cache[n];
}
