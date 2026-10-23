/*
 * A019958  Expansion of 1/((1-5*x)*(1-7*x)*(1-10*x)).
 * https://oeis.org/A019958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - 155*a(n-2) + 350*a(n-3); a(0)=1, a(1)=22,
 *            a(2)=329
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;      /* cache of computed terms */
static idx_t  cache_cap  = 0;        /* allocated capacity (in elements) */
static idx_t  computed   = 0;        /* number of terms currently stored (first computed‑len indices) */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n */
    if (cache_cap < n + 1) {
        /* Grow to at least 3 to store the base cases */
        idx_t new_cap = (cache_cap == 0) ? 3 : cache_cap * 2;
        while (new_cap < n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* out‑of‑memory: return 0 (unlikely in tests) */
            return (seq_t)0;
        }
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms once */
    if (computed == 0) {
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)22;
        seq_cache[2] = (seq_t)329;
        computed = 3;
    }

    /* If the desired index is already computed, return it */
    if (n < computed)
        return seq_cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t a1 = seq_cache[i - 1];
        seq_t a2 = seq_cache[i - 2];
        seq_t a3 = seq_cache[i - 3];
        seq_t term = 22 * a1;
        term -= 155 * a2;
        term += 350 * a3;
        seq_cache[i] = term;
    }
    computed = n + 1;

    return seq_cache[n];
}
