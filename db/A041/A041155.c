/*
 * A041155  Denominators of continued fraction convergents to sqrt(87).
 * https://oeis.org/A041155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0) = 1, a(1) = 3, a(2) = 55, a(3) = 168 a(n) = 56*a(n-2) -
 *            a(n-4) for n >= 4 (Denominators of continued fraction
 *            convergents to sqrt(87))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed terms */
static idx_t cache_size = 0;      /* number of terms currently stored (cache_size == cache_size) */

/* Ensures terms up to index n are computed and stored in cache. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache;

    if (cache == NULL) {                 /* first allocation */
        new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    } else {                              /* extend existing array */
        new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    }

    if (new_cache == NULL) {              /* allocation failure */
        exit(EXIT_FAILURE);
    }

    /* compute new terms that were not yet stored */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            new_cache[i] = (seq_t)1;
        } else if (i == 1) {
            new_cache[i] = (seq_t)3;
        } else if (i == 2) {
            new_cache[i] = (seq_t)55;
        } else if (i == 3) {
            new_cache[i] = (seq_t)168;
        } else {
            /* recurrence: a(i) = 56*a(i-2) - a(i-4) */
            new_cache[i] = new_cache[i-2] * (seq_t)56 - new_cache[i-4];
        }
    }

    cache      = new_cache;
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
