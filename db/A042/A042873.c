/*
 * A042873  Denominators of continued fraction convergents to sqrt(968).
 * https://oeis.org/A042873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 39202*a(n‑6) - a(n‑12) for n > 11 * Initial values: *
 *            a(0)=1, a(1)=8, a(2)=9, a(3)=62, a(4)=71, a(5)=630, *
 *            a(6)=39131, a(7)=313678, a(8)=352809, a(9)=2430532, *
 *            a(10)=2783341, a(11)=24697260
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *cache      = NULL;   /* array of computed terms           */
static size_t cache_len  = 0;      /* number of terms stored in cache   */
static size_t cache_cap  = 0;      /* allocated capacity (in terms)     */

/* Ensure that cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_len) return;          /* already satisfied */

    /* Expand capacity if necessary */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        if (!new_cache) abort();                 /* out of memory */
        cache = new_cache;
        cache_cap = required;
    }

    /* Initial values for indices 0 … 11 */
    static const seq_t init[12] = { (seq_t)1, (seq_t)8, (seq_t)9,
                                     (seq_t)62, (seq_t)71, (seq_t)630,
                                     (seq_t)39131, (seq_t)313678,
                                     (seq_t)352809, (seq_t)2430532,
                                     (seq_t)2783341, (seq_t)24697260 };

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 12) {
            cache[i] = init[i];
        } else {
            /* recurrence: a(i) = 39202*a(i‑6) – a(i‑12) */
            cache[i] = ((seq_t)39202 * cache[i-6]) - cache[i-12];
        }
    }

    cache_len = required;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
