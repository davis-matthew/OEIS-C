/*
 * A076905  a(1)=1, a(n)=n-a(n-1)+a(a(n-1))-a(a(a(n-1))).
 * https://oeis.org/A076905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(n) = n - a(n-1) + a(a(n-1)) - a(a(a(n-1)))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises all computed terms in a 1‑based dynamic array.
   The array grows using standard realloc so it works both with C and C++ builds.
   All arithmetic is performed in the type `seq_t`; conversions from `idx_t`
   to `seq_t` (and vice versa) are permitted by the problem statement. */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

static seq_t *cache      = NULL; /* 1‑based array of terms */
/* Largest n having a computed value */
static idx_t  max_cached = 0;
/* Current allocated capacity (elements, not bytes). Allocated array size is cache_cap + 1. */
static size_t cache_cap  = 0;

/* ---- helper: grow the cache to hold at least `needed` indices (1‑based) ---- */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return; /* already sufficient */

    size_t new_cap = cache_cap ? cache_cap << 1 : 16;          /* double or start at 16 */
    if (new_cap < needed)
        new_cap = needed;

    /* Allocate new larger array */
    seq_t *new_cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – abort. The problem environment usually
           terminates the program on such fatal errors. */
        exit(EXIT_FAILURE);
    }

    /* Copy existing data to new array */
    if (cache != NULL) {
        for (size_t i = 1; i <= cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache       = new_cache;
    cache_cap   = new_cap;
}

/* ---- the required OEIS sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0; return zero to avoid UB, though offset is 1 */

    /* Ensure cache large enough */
    if ((size_t)n + 1 > cache_cap)
        grow_cache((size_t)n + 1);

    /* Initialise cache on first call */
    if (max_cached == 0) {
        cache[1] = (seq_t)1;
        max_cached = 1;
    }

    /* Return cached value if already computed */
    if (n <= max_cached)
        return cache[n];

    /* Compute terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i)
    {
        seq_t prev       = cache[i - 1];          /* a(i-1)   */
        idx_t idx_prev   = (idx_t)prev;           /* safe: a(i-1) < i  */
        seq_t a_prev     = cache[idx_prev];       /* a(a(i-1)) */
        idx_t idx_a_prev = (idx_t)a_prev;         /* safe: a(a(i-1)) < i-1 */
        seq_t a_aa_prev  = cache[idx_a_prev];     /* a(a(a(i-1))) */

        seq_t val = (seq_t)i;
        val += a_prev;           /* + a(a(i-1)) */
        val -= prev;             /* - a(i-1)    */
        val -= a_aa_prev;        /* - a(a(a(i-1))) */

        cache[i] = val;
    }

    max_cached = n;
    return cache[n];
}
