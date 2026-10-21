/*
 * A206625  Expansion of x * (1 + x) * (1 - x^2) * (1 + x^3) / (1 - 2*x^2 -
 *          2*x^4 - 2*x^6 + x^8) in powers of x.
 * https://oeis.org/A206625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑2) + 2*a(n‑4) + 2*a(n‑6) – a(n‑8)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 44, u64 n <= 86, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=0, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=5, a(6)=5, a(7)=13 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic cache of computed terms */
static size_t cache_cap = 0;       /* allocated capacity of cache   */
static size_t cache_len = 0;       /* number of terms currently stored */

/* Base values for indices 0 … 7 */
static const seq_t base_vals[8] = {0, 1, 1, 1, 2, 5, 5, 13};

/* Expand the cache to accommodate indices up to `need-1` (i.e. `need` terms). */
static void expand_cache(size_t need)
{
    if (need <= cache_cap) return;            /* already big enough */

    /* At least 8 terms are required for the recurrence */
    if (need < 8) need = 8;

    /* Choose a new capacity: double the current or the required size, whichever is larger */
    size_t new_cap = (cache_cap == 0) ? need : cache_cap;
    while (new_cap < need) new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);   /* allocation failed */

    /* Initialize newly allocated slots with base values if they fall within the first 8 indices */
    if (cache_len < 8) {
        size_t start = cache_len;
        if (new_cap > 8) start = 8;
        for (size_t i = cache_len; i < start; ++i) {
            new_cache[i] = base_vals[i];
        }
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    /* Handle negative indices trivially (they are not requested). */
    if (n == (idx_t) -1) return 0;  /* dummy to avoid warnings */

    size_t idx = (size_t)n;

    /* Ensure that the cache contains at least the first `idx+1` terms. */
    if (idx >= cache_len) {
        expand_cache(idx + 1);

        /* Compute new terms only from the first missing index onwards. */
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i < 8) {                     /* base values */
                cache[i] = base_vals[i];
            } else {
                cache[i] = 2 * cache[i - 2]
                         + 2 * cache[i - 4]
                         + 2 * cache[i - 6]
                         - cache[i - 8];
            }
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
