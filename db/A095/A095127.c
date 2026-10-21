/*
 * A095127  a(n+3) = 2*a(n+2) + 3*a(n+1) - a(n); with a(1) = 1, a(2) = 4, a(3)
 *          = 10.
 * https://oeis.org/A095127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 2*a(n+2) + 3*a(n+1) - a(n), with a(1)=1, a(2)=4,
 *            a(3)=10
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function caches computed values for reuse. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of cached terms; index 0 unused */
static idx_t   cache_cap = 0;    /* largest index currently stored */

/* Compute and store all terms up to n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= cache_cap) return;        /* already have them */

    /* enlarge the cache */
    idx_t new_cap = n;
    seq_t *new_cache = (seq_t *)realloc(cache,
                                      (new_cap + 1) * sizeof(seq_t)); /* +1 to use 1‑based indexing */
    if (!new_cache) {
        /* allocation failed – abort with a non‑zero exit status */
        abort();
    }
    cache = new_cache;

    /* compute new terms */
    for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if (i == 2) {
            cache[2] = (seq_t)4;
        } else if (i == 3) {
            cache[3] = (seq_t)10;
        } else {
            cache[i] = (seq_t)2 * cache[i-1] + (seq_t)3 * cache[i-2] - cache[i-3];
        }
    }

    cache_cap = new_cap;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; 0 is undefined */
    compute_up_to(n);
    return cache[n];
}
