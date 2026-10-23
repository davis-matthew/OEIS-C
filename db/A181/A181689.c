/*
 * A181689  Number of maximal self-avoiding walks from NW to SW corners of a 5
 *          X n grid.
 * https://oeis.org/A181689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 11*a(n-2) + 2*a(n-6) for n>6 Initial values: a(1)=1,
 *            a(2)=0, a(3)=8, a(4)=0, a(5)=86, a(6)=0
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed = 0; /* largest n for which cache[n] is valid */

/* Ensure cache has at least 'size' elements (indices 0..size-1) */
static void ensure_cache_size(idx_t size)
{
    if ((size_t)size <= cache_size)
        return;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)size * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    /* Zero the new part */
    for (idx_t i = cache_size; i < size; ++i)
        tmp[i] = 0;
    cache = tmp;
    cache_size = size;
}

/* Initialise the known base values (indices 1..6).  computed is left at 6. */
static void init_base(void)
{
    if (computed >= 6)
        return;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)0;
    cache[3] = (seq_t)8;
    cache[4] = (seq_t)0;
    cache[5] = (seq_t)86;
    cache[6] = (seq_t)0;
    computed = 6;
}

/* Compute values up to n, storing them in cache. */
static void compute_up_to(idx_t n)
{
    if (n <= computed)
        return;
    /* Ensure we have room for indices 0..n */
    idx_t needed = n + 1;
    if (needed < 7)
        needed = 7;          /* enough for the base values */
    ensure_cache_size(needed);

    if (computed < 6)
        init_base();

    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Base values already set for i <= 6 */
        if (i <= 6)
            continue;
        /* Recurrence: a(i) = 11*a(i-2) + 2*a(i-6) */
        cache[i] = (seq_t)(11 * cache[i - 2] + 2 * cache[i - 6]);
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined below offset, but safe */
    compute_up_to(n);
    return cache[n];
}
