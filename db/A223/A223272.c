/*
 * A223272  Rolling cube footprints: number of 4Xn 0..5 arrays starting with 0
 *          where 0..5 label faces of a cube and every array movement to a
 *          horizontal, diagonal or antidiagonal neighbor moves across a
 *          corresponding cube edge.
 * https://oeis.org/A223272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 48*a(n-1) + 4096*a(n-2) - 12288*a(n-3) - 1638400*a(n-4) *
 *            + 2097152*a(n-5) + 67108864*a(n-6) for n > 7 * Initial terms:
 *            a(1)=216, a(2)=6912, a(3)=765952, a(4)=63438848, *
 *            a(5)=5889851392, a(6)=522106961920, a(7)=47175115472896
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Base values for n = 1..7.  cache[0] is unused. */
static const seq_t init_vals[8] = {
    0ULL,                     /* index 0, unused */
    216ULL,                   /* a(1) */
    6912ULL,                  /* a(2) */
    765952ULL,                /* a(3) */
    63438848ULL,              /* a(4) */
    5889851392ULL,            /* a(5) */
    522106961920ULL,          /* a(6) */
    47175115472896ULL         /* a(7) */
};

/* Static cache for memoization */
static seq_t *cache = NULL;          /* dynamic array of computed terms */
static size_t cache_size = 0;        /* allocated size in elements */
static idx_t max_computed = 0;       /* highest n for which cache[n] is valid */

/* Ensure the cache has room for indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > 0 && cache_size > (size_t)n + 1) return;
    size_t new_size = (cache_size < 16) ? 16 : cache_size;
    while (new_size <= (size_t)n + 1) new_size <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure – stop execution */
    cache = tmp;
    cache_size = new_size;
    /* Zero new locations to avoid garbage reads */
    for (size_t i = max_computed + 1; i < new_size; ++i) cache[i] = 0;
}

/* Compute a(n) using memoized recurrence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n=0, but return 0 */
    ensure_cache_size(n);
    if (n <= max_computed) return cache[n];

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 7) {
            cache[i] = init_vals[(size_t)i];
        } else {
            seq_t term = (seq_t)48ULL * cache[i - 1];
            term += (seq_t)4096ULL * cache[i - 2];
            term += (seq_t)2097152ULL * cache[i - 5];
            term += (seq_t)67108864ULL * cache[i - 6];
            term -= (seq_t)12288ULL * cache[i - 3];
            term -= (seq_t)1638400ULL * cache[i - 4];
            cache[i] = term;
        }
    }
    max_computed = n;
    return cache[n];
}
