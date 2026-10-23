/*
 * A193147  Expansion of 1/(1 - x - 2*x^3 - x^5).
 * https://oeis.org/A193147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + a(n-5) with a(0) = 1, a(n) = 0 for n
 *            < 0
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamic cache of computed terms */
static idx_t  cache_size = 0;        /* allocated size of cache   */
static idx_t  computed_up_to = 0;    /* highest index already computed (≥0) */

/* Compute a single term using the recurrence, assuming previous terms
 * are already in cache   */
static inline seq_t compute_term(idx_t i)
{
    seq_t val = 0;

    /* a(i-1) */
    if (i >= 1)
        val += cache[i - 1];

    /* 2 * a(i-3) */
    if (i >= 3) {
        seq_t tmp = cache[i - 3];
        tmp += tmp;          /* multiply by 2 */
        val += tmp;
    }

    /* a(i-5) */
    if (i >= 5)
        val += cache[i - 5];

    return val;
}

/* Public function requested by the build system */
seq_t a(idx_t n)
{
    /* extend cache to accommodate index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise the first term if this is the first call */
    if (computed_up_to == 0 && cache_size > 0) {
        cache[0] = (seq_t)1;         /* a(0) = 1   */
        /* terms a(-1), a(-2), a(-3), a(-4) are logically 0 and need no storage */
    }

    /* compute any missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
        cache[i] = compute_term(i);

    computed_up_to = (n > computed_up_to) ? n : computed_up_to;
    return cache[n];
}
