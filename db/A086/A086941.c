/*
 * A086941  a(n) = k where R(k+6) = 3.
 * https://oeis.org/A086941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*(10^n - 2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=24, a(2)=294, a(n)=11*a(n-1)-10*a(n-2) for n>2 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* Ensure the cache has capacity for at least 'need' terms (index based) */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_cache) {
        /* Allocation failed – aborting as per standard behaviour */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)             /* undefined; return 0 as a placeholder */
        return (seq_t)0;

    /* Adjust zero‑based index for internal storage */
    idx_t idx = n - 1;

    /* Allocate or grow cache as needed */
    if (idx >= cache_size) {
        ensure_cache_cap(idx + 1);
        for (idx_t i = cache_size; i <= idx; ++i) {
            if (i == (idx_t)0) {
                cache[0] = (seq_t)24;
            } else if (i == (idx_t)1) {
                cache[1] = (seq_t)294;
            } else {
                seq_t prev1 = cache[i - 1];
                seq_t prev2 = cache[i - 2];
                /* a(i+1) = 11*a(i) - 10*a(i-1) */
                seq_t term = (seq_t)11 * prev1 - (seq_t)10 * prev2;
                cache[i] = term;
            }
        }
        cache_size = idx + 1;
    }

    return cache[idx];
}
