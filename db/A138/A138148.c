/*
 * A138148  Cyclops numbers with binary digits only.
 * https://oeis.org/A138148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = (10^(1+2*n) - 9*10^n - 1)/9 Recurrence: a(0)=0, a(1)=101,
 *            a(2)=11011, a(n)=111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) (n>2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;           /* cache of already computed terms   */
static size_t cache_capacity = 0;     /* allocated capacity of the cache   */
static size_t computed_terms = 0;     /* number of terms that have been filled */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /* First call: allocate a minimal cache and fill the base cases   */
    if (cache == NULL) {
        /* Ensure at least three slots to store a(0..2)                */
        cache_capacity = (idx > 2 ? idx + 1 : 3);
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache) return 0;                     /* allocation failed   */
        cache[0] = 0;
        cache[1] = 101;
        cache[2] = 11011;
        computed_terms = 3;                       /* a(0), a(1), a(2) are ready */
    }

    /* Grow the cache if the requested index is beyond current size   */
    if (idx >= cache_capacity) {
        size_t new_cap = cache_capacity * 2 + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failed   */
        cache       = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms up to the requested index                */
    if (idx >= computed_terms) {
        for (size_t i = computed_terms; i <= idx; ++i) {
            seq_t t1 = cache[i - 1] * (seq_t)111;
            seq_t t2 = cache[i - 2] * (seq_t)1110;
            seq_t t3 = cache[i - 3] * (seq_t)1000;
            seq_t val = t1 - t2;                /* t1 ≥ t2 for all n ≥ 3 */
            val += t3;
            cache[i] = val;
        }
        computed_terms = idx + 1;
    }

    return cache[idx];
}
