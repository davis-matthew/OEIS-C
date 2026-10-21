/*
 * A089277  Sixth column of the triangle A071951 (Legendre-Stirling).
 * https://oeis.org/A089277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence * * a(0) = 1 * a(1) = 112 *
 *            a(2) = 7896 * a(3) = 453056 * a(4) = 23232176 * a(5) =
 *            1113673728 * * a(n) = 112 a(n-1) - 4648 a(n-2) + 89280 a(n-3) *
 *            - 808848 a(n-4) + 3110400 a(n-5) - 3628800 a(n-6) (n ≥ 6) *
 *            where all operations are performed modulo the natural...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdlib.h>     /* malloc, realloc */

static const seq_t E1 = (seq_t)112;
static const seq_t E2 = (seq_t)4648;
static const seq_t E3 = (seq_t)89280;
static const seq_t E4 = (seq_t)808848;
static const seq_t E5 = (seq_t)3110400;
static const seq_t E6 = (seq_t)3628800;

/* Initial six terms */
static const seq_t INIT[6] = {
    (seq_t)1,
    (seq_t)112,
    (seq_t)7896,
    (seq_t)453056,
    (seq_t)23232176,
    (seq_t)1113673728
};

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Expand the cache to at least need terms (need >= 0) */
static void expand_cache(idx_t need)
{
    if (cache_len > need)
        return;
    /* Allocate or reallocate */
    idx_t new_len   = need + 1;
    seq_t *new_cache;
    if (cache == NULL) {
        new_cache = (seq_t *)malloc(sizeof(seq_t) * new_len);
    } else {
        new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    }
    cache = new_cache;
    /* Populate newly allocated entries */
    for (idx_t i = cache_len; i <= need; ++i) {
        if (i < 6) {
            cache[(size_t)i] = INIT[(size_t)i];
        } else {
            /* recurrence */
            seq_t val =    E1 * cache[(size_t)(i - 1)]
                         - E2 * cache[(size_t)(i - 2)]
                         + E3 * cache[(size_t)(i - 3)]
                         - E4 * cache[(size_t)(i - 4)]
                         + E5 * cache[(size_t)(i - 5)]
                         - E6 * cache[(size_t)(i - 6)];
            cache[(size_t)i] = val;
        }
    }
    cache_len = new_len;
}

seq_t a(idx_t n) {
    expand_cache(n);
    return cache[(size_t)n];
}
