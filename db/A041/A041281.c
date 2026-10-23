/*
 * A041281  Denominators of continued fraction convergents to sqrt(153).
 * https://oeis.org/A041281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 4354*a(n-8) - a(n-16) (for n >= 16) Initial values for 0
 *            <= n < 16 are listed explicitly below
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 86,
 *            big any n
 * Helpers:   mul4354
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul4354.h"

/* Persistent cache of already computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;          /* number of valid entries in cache */

/* First 16 terms of the sequence.  They are stored at start-up. */
static const seq_t init_terms[16] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)8,
    (seq_t)19, (seq_t)46, (seq_t)65, (seq_t)176,
    (seq_t)4289,(seq_t)8754,(seq_t)13043,(seq_t)34840,
    (seq_t)82723,(seq_t)200286,(seq_t)283009,(seq_t)766304
};

/* Return the n-th term of the sequence, computing on demand. */
seq_t a(idx_t n)
{
    /* Initialise cache with the first 16 terms on first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(16 * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failed */
        for (size_t i = 0; i < 16; ++i) cache[i] = init_terms[i];
        cache_len = 16;
    }

    /* If already computed, return immediately. */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* Grow cache to hold terms up to n. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failed */
        cache = tmp;
        /* cache_len will be updated after computing new terms */
    }

    /* Compute missing terms from the first uncomputed index onwards. */
    size_t start = cache_len;       /* first index that is not yet written */
    if (start < 16) start = 16;     /* all indices <16 are already known */

    for (size_t i = start; i <= (size_t)n; ++i) {
        cache[i] = mul4354(cache[i - 8]) - cache[i - 16];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
