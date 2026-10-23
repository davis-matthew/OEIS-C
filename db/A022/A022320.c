/*
 * A022320  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 6.
 * https://oeis.org/A022320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 1, a(1) = 6
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;           /* array holding computed terms */
static size_t cache_cap = 0;          /* current allocation size */
static idx_t  computed_max = 0;       /* index of the largest term computed */

/* Return the n‑th term of OEIS sequence A022320 */
seq_t a(idx_t n)
{
    /* Base cases handled directly to avoid array accesses */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        computed_max = 1;
    }

    /* Extend the cache if required */
    if (n > computed_max) {
        /* Ensure capacity for indices 0 … n */
        if ((size_t)(n + 1) > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 2;
            while ((size_t)(n + 1) > new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;          /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute missing terms iteratively */
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2] + (seq_t)1;
        }
        computed_max = n;
    }

    return cache[n];
}
