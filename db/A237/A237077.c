/*
 * A237077  Number of (n+1)X(1+1) 0..2 arrays colored with the maximum plus
 *          the minimum minus the upper median minus the lower median of every
 *          2X2 subblock.
 * https://oeis.org/A237077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237077: number of (n+1)×2 arrays colored with 0..2 such that
 * for every 2×2 subblock the equality
 *   max+min = upper_median+lower_median
 * holds.
 *
 * Formula (empirical recurrence):
 *   a(n) = 76·a(n‑3) + 544·a(n‑4) + 1272·a(n‑5) + 3512·a(n‑6) + 3568·a(n‑7)
 *          – 2944·a(n‑8) – 4864·a(n‑9) – 6144·a(n‑10)      (n ≥ 11)
 *
 * Initial values:
 *   a(1)=81, a(2)=484, a(3)=2928, a(4)=17644, a(5)=106528,
 *   a(6)=641736, a(7)=3871696, a(8)=23356112, a(9)=140860224,
 *   a(10)=849369952
 *
 * The implementation evaluates the recurrence and caches results for
 * arbitrary order of calls and for n up to at least 10 000.  All arithmetic
 * is performed on the generic integer type seq_t.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* 1‑based array of computed values */
static idx_t cache_size = 0;    /* allocated size (highest index + 1) */
static idx_t computed_max = 0;  /* highest index with a(n) computed */

/* Initial 10 terms (1‑based).  Index 0 is unused. */
static const seq_t initial_terms[11] = {
    0ULL,
    81ULL,       /* a(1)  */
    484ULL,      /* a(2)  */
    2928ULL,     /* a(3)  */
    17644ULL,    /* a(4)  */
    106528ULL,   /* a(5)  */
    641736ULL,   /* a(6)  */
    3871696ULL,  /* a(7)  */
    23356112ULL, /* a(8)  */
    140860224ULL,/* a(9)  */
    849369952ULL /* a(10) */
};

/* Ensure that cache can hold indices up to n.  New elements are zeroed. */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = n + 1;          /* indices 0..n inclusive */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* allocation failure */

    /* initialise freshly allocated part to 0 */
    for (idx_t i = cache_size; i < new_size; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_size = new_size;
}

/* Expand cache to n and compute any missing terms by the recurrence. */
static void compute_to(idx_t n)
{
    if (n <= computed_max)
        return;                      /* nothing to do */

    /* produce terms for i = computed_max+1 .. n */
    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        /* recurrence uses indices i-3 ... i-10;
           those indices are guaranteed to exist because
           computed_max ≥ 10 before entering this loop.  */
        seq_t term =
            (seq_t)76   * cache[i - 3] +
            (seq_t)544  * cache[i - 4] +
            (seq_t)1272 * cache[i - 5] +
            (seq_t)3512 * cache[i - 6] +
            (seq_t)3568 * cache[i - 7] -
            (seq_t)2944 * cache[i - 8] -
            (seq_t)4864 * cache[i - 9] -
            (seq_t)6144 * cache[i -10];
        cache[i] = term;
    }
    computed_max = n;
}

/* Main entry point: return a(n) for n >= 1.  Undefined for n==0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* graceful handling of offset */

    /* initialise on first use */
    if (cache == NULL)
    {
        ensure_cache(10);           /* make space for indices 0..10 */
        for (idx_t i = 0; i <= 10; ++i)
            cache[i] = initial_terms[i];
        computed_max = 10;
    }

    if (n <= computed_max)
        return cache[n];

    ensure_cache(n);
    compute_to(n);
    return cache[n];
}
