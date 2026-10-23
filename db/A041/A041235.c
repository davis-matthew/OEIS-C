/*
 * A041235  Denominators of continued fraction convergents to sqrt(129).
 * https://oeis.org/A041235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to √129
   The CF of √129 is [11; 2,1,3,1,6,1,3,1,2,22, …] repeating.
   We compute q_k via q_k = a_k·q_{k‑1} + q_{k‑2} with
   q_{‑2}=1, q_{‑1}=0.  The recurrence a(n)=33710·a(n‑10)–a(n‑20)
   is a consequence of this continued‑fraction structure. */
#include <stdlib.h>

static const int period[10] = {2,1,3,1,6,1,3,1,2,22};

/* Helper to obtain a_k for the continued fraction of √129 */
static inline seq_t cf_term(idx_t k)
{
    if (k == 0)
        return (seq_t)11;
    return (seq_t)period[(size_t)(k - 1) % 10];
}

seq_t a(idx_t n)
{
    /* Static cache of already computed denominators */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;  /* number of terms stored (always >0) */
    static size_t cache_cap  = 0;  /* capacity of the cache array */

    /* Lazy initialization: allocate first block and store q_0 = 1 */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory, unlikely in tests */
        cache[0] = (seq_t)1;                 /* q_0 = 1 */
        cache_size = 1;
    }

    /* Grow capacity if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;        /* double until it fits */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to index n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t a_i = cf_term((idx_t)i);   /* continued‑fraction coefficient */

        seq_t term;
        if (i == 1) {
            /* q_1 = a_1 · q_0 + q_{‑1} ,  q_{‑1}=0 */
            term = a_i * cache[0];
        } else {
            term = a_i * cache[i - 1] + cache[i - 2];
        }
        cache[i] = term;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
