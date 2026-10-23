/*
 * A116525  a(0)=1, a(1)=1, a(n) = 11*a(n/2) for even n, and a(n) =
 *          10*a((n-1)/2) + a((n+1)/2) for odd n >= 3.
 * https://oeis.org/A116525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n)=11*a(n/2) for even n,
 *            a(n)=10*a((n-1)/2)+a((n+1)/2) for odd n≥3
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 767, u64 n <= 9241,
 *            u128 n <= 9241, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_seq.h"

/* Cache of computed terms; initialized lazily. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;       /* allocated size of cache array */
static idx_t cached_max = 0;      /* highest index already computed */
static bool init_done = false;

/* Ensure space for at least 'needed' elements. */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache)
        return; /* allocation failed; caller will handle */

    if (cache) {
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache); /* free_seq only for C++ builds; free works in both */
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute all terms up to index n inclusively. */
static void compute_up_to(idx_t n)
{
    if (!init_done) {
        /* Initial state: a(0)=0, a(1)=1 */
        cache_cap = 2;
        cache = alloc_seq(cache_cap);
        if (!cache)
            return;
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cached_max = 1;
        init_done = true;
    }

    if (n <= cached_max)
        return;

    ensure_capacity(n + 1);

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if ((i & 1U) == 0) {           /* even index */
            cache[i] = ((seq_t)11) * cache[i / 2];
        } else if (i == 1) {           /* odd index 1 already set */
            /* nothing to do */
        } else {                       /* odd index ≥ 3 */
            cache[i] = ((seq_t)10) * cache[(i - 1) / 2]
                     + cache[(i + 1) / 2];
        }
    }
    cached_max = n;
}

/* Sequence function required by the framework. */
seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
