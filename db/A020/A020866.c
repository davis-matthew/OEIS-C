/*
 * A020866  Number of strong edge-subgraphs in Moebius ladder M_n.
 * https://oeis.org/A020866
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 9*a(n-2) + a(n-3) - 2*a(n-4), a(2)=41,
 *            a(3)=265, a(4)=1697, a(5)=10897
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* array of computed values */
static idx_t  cache_cap  = 0;      /* allocated size (highest index possible) */
static idx_t  cache_len  = 0;      /* highest index that has been computed */

/* Ensure cache can address indices up to cap (inclusive). */
static void ensure_capacity(idx_t cap)
{
    if (cap <= cache_cap) return;
    seq_t *new_cache = (seq_t *)realloc(cache, (cap + 1) * sizeof(seq_t));
    if (!new_cache) return;             /* allocation failure – leave cached data */
    /* zero new entries */
    for (idx_t i = cache_cap + 1; i <= cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = cap;
}

/* Store the known initial values for the recurrence. */
static void set_base_values(void)
{
    cache[2] = (seq_t)41;
    cache[3] = (seq_t)265;
    cache[4] = (seq_t)1697;
    cache[5] = (seq_t)10897;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;                /* below offset – behaviour arbitrary */

    /* We need at least indices 2…5 to start the recurrence. */
    idx_t need = n;
    if (need < 5) need = 5;

    if (!cache) {
        ensure_capacity(need);
        set_base_values();
        cache_len = 5;
    } else if (need > cache_cap) {
        ensure_capacity(need);
        set_base_values();                     /* values are idempotent */
    }

    /* Compute any missing terms up to n. */
    if (cache_len < n) {
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            cache[k] = 5 * cache[k - 1]
                     + 9 * cache[k - 2]
                     +     cache[k - 3]
                     - 2 * cache[k - 4];
        }
        cache_len = n;
    }

    return cache[n];
}
