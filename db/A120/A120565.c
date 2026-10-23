/*
 * A120565  Maximum over all planar partitions of n of the number of ways the
 *          partition can be shrunk by removing a single element.
 * https://oeis.org/A120565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   let m be the largest integer such that m*(m+1)*(m+2)/6 <= n
 *            a(0)=0, a(1)=1, a(2)=1 for n>2: a(n)=max( m*(m+1)/2 , (m+1) +
 *            a(n-(m+1)*(m+2)/2) ) (a(k)=0 for k<0)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;   /* array of a(k) values */
static size_t cache_len = 0;  /* number of entries currently stored */

/* Expand the cache to hold indices up to n (inclusive).  The cache grows
   geometrically to keep amortised cost at O(1) per store. */
static void
ensure_cache(idx_t n)
{
    if (cache_len <= (size_t)n) {
        size_t new_cap = cache_len > 0 ? cache_len : 64;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;                      /* grow until we can hold n */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* out‑of‑memory */
        cache = tmp;
        /* initialise any newly allocated slots to 0 */
        for (size_t i = cache_len; i < new_cap; ++i)
            cache[i] = 0;
        cache_len = new_cap;
    }
}

/* Initialize base values a(0)=0, a(1)=1, a(2)=1 if they are not already
   present in the cache. */
static void
initialise_base(void)
{
    if (cache[0] == 0 && cache[1] == 0 && cache[2] == 0) {
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 1;
    }
}

/* Return the nth term of the sequence.  The function is thread‑unsafe,
   but no threading support is required by the specification. */
seq_t
a(idx_t n)
{
    if (cache == NULL) {
        cache = (seq_t *)malloc(64 * sizeof(seq_t));
        if (!cache) abort();
        for (size_t i = 0; i < 64; ++i)
            cache[i] = 0;
        cache_len = 64;
    }

    /* expand cache if necessary and initialise base terms */
    ensure_cache(n);
    initialise_base();

    /* Compute any missing values up to n */
    for (idx_t i = 3; i <= n; ++i) {
        /* skip if already computed (maybe from a previous call) */
        if (cache[(size_t)i] != 0 || i == 0 || i == 1 || i == 2)
            continue;

        /* find largest m with tetrahedral(m) <= i */
        idx_t m = 0;
        for (;; ++m) {
            idx_t t = m * (m + 1) * (m + 2) / 6;
            if (t > i) {
                --m;
                break;
            }
        }

        /* value1 = m*(m+1)/2 */
        seq_t value1 = (seq_t)m * ((seq_t)m + 1) / 2;

        /* delta = (m+1)*(m+2)/2 */
        idx_t delta = (m + 1) * (m + 2) / 2;

        seq_t value2;
        if ((size_t)i >= (size_t)delta) {
            idx_t sub = i - delta;
            value2 = (seq_t)(m + 1) + cache[(size_t)sub];
        } else {
            value2 = (seq_t)(m + 1);
        }

        cache[(size_t)i] = value1 > value2 ? value1 : value2;
    }

    return cache[(size_t)n];
}
