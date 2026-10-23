/*
 * A016157  Expansion of g.f. 1/((1-4*x)*(1-10*x)).
 * https://oeis.org/A016157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*10^n - 2*4^n)/3. Implemented by the linear recurrence:
 *            a(0) = 1, a(1) = 14, a(n) = 14*a(n-1) - 40*a(n-2) (for n ≥ 2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

static seq_t *cache    = NULL;   /* Cached values a(0) … a(m) */
static size_t cache_cap = 0;     /* Allocated capacity of cache   */
static size_t cache_len = 0;     /* Number of elements actually stored */

/* Ensure cache has space for indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    idx_t needed = n + 1;
    if ((size_t)needed <= cache_cap)
        return;

    size_t new_cap = (size_t)(needed * 2);
    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache) /* allocation failure – aborting is acceptable */
        abort();

    /* Copy existing data, if any */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    if (cache)
        free_seq(cache);

    cache     = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        ensure_cache_size(0);
        if (cache_len == 0) {
            cache[0] = (seq_t)1;
            cache_len = 1;
        }
        return cache[0];
    }
    if (n == 1) {
        ensure_cache_size(1);
        if (cache_len <= 1) {
            if (cache_len == 0) cache[0] = (seq_t)1;
            cache[1] = (seq_t)14;
            cache_len = 2;
        }
        return cache[1];
    }

    ensure_cache_size(n);

    /* Compute missing values up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)(( (seq_t)14 * prev) - ((seq_t)40 * prev2));
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
