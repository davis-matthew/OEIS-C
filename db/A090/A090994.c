/*
 * A090994  Number of meaningful differential operations of the n-th order on
 *          the space R^9.
 * https://oeis.org/A090994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 31, u64 n <= 65, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(k+5) = a(k+4) + 4*a(k+3) - 3*a(k+2) - 3*a(k+1) + a(k);
 * Initial values: a(1)=9, a(2)=17, a(3)=32, a(4)=61, a(5)=116.
 * The sequence is produced via the recurrence; a cache of previously
 * computed values is kept to allow arbitrary call order.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache of computed terms.  cache[0] corresponds to a(1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of terms currently stored */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Make sure the cache can hold at least 'need' terms. */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *newptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (newptr)
        cache = newptr;            /* realloc succeeded */
    cache_cap = new_cap;
}

/* Initialise the cache with the first five terms. */
static void init_base(void)
{
    const seq_t base[5] = {9, 17, 32, 61, 116};
    ensure_capacity(5);
    for (size_t i = 0; i < 5; ++i)
        cache[i] = base[i];
    cache_len = 5;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n < 1 */

    /* Initialise cache only once. */
    if (cache == NULL)
        init_base();

    /* If already computed, return cached value. */
    if (n <= cache_len)
        return cache[(size_t)(n - 1)];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 5)          /* first five terms already stored */
            continue;

        /* Recurrence: a(i) = a(i-1) + 4*a(i-2) - 3*a(i-3) - 3*a(i-4) + a(i-5) */
        seq_t val = cache[(size_t)(i - 2)]   /* a(i-1) */
                  + 4 * cache[(size_t)(i - 3)]   /* 4*a(i-2) */
                  - 3 * cache[(size_t)(i - 4)]   /* 3*a(i-3) */
                  - 3 * cache[(size_t)(i - 5)]   /* 3*a(i-4) */
                  + cache[(size_t)(i - 6)];      /* a(i-5) */

        ensure_capacity(i);
        cache[(size_t)(i - 1)] = val;
    }

    cache_len = n;   /* update length after computation */
    return cache[(size_t)(n - 1)];
}
