/*
 * A170857  a(0) = 0, a(1) = 1; a(2^i + j) = 3a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(2^i + j) = 3 * a(j) + 3 * a(j + 1) for 0
 *            <= j < 2^i
 * Range:     u8 n <= 13, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* table of already computed values */
static size_t cache_cap = 0;    /* allocated capacity of the table   */
static idx_t  computed = 1;     /* highest index that has been computed (a[0] and a[1] known) */

/* Return the highest power of two <= n, as a idx_t */
static idx_t highest_pow2_leq(idx_t n)
{
    idx_t p = 0;
    while (((idx_t)1 << (p + 1)) <= n)
        ++p;
    return (idx_t)1 << p;
}

/* Ensure the cache has room for at least n+1 entries and compute any
   missing values up to n.  The cache stores values for all indices up
   to 'computed', and any indices beyond that are set to zero until
   explicitly computed. */
static void ensure_cache(idx_t n)
{
    /* First, allocate the cache if it does not yet exist. */
    if (cache == NULL)
    {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Grow the allocated capacity if needed. */
    if ((size_t)(n + 1) > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;              /* out‑of‑memory; abort silently */
        /* Initialize new slots to zero (they will be overwritten when computed). */
        for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any new terms beyond the current 'computed' index up to n. */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        idx_t mask = highest_pow2_leq(i);
        idx_t j = i - mask;                 /* 0 <= j < mask */
        seq_t aj  = cache[j];
        seq_t aj1 = cache[j + 1];
        cache[i] = (seq_t)3 * aj + (seq_t)3 * aj1;
    }
    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if (n <= (idx_t)computed)
        return cache[n];

    ensure_cache(n);
    return cache[n];
}
