/*
 * A063306  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          33 ).
 * https://oeis.org/A063306
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - a(n-5) + a(n-7) - a(n-8) for
 *            all n > 11. Initial values: a(2) = -1, a(3)=19, a(4)=50,
 *            a(5)=80, a(6)=110, a(7)=138, a(8)=168, a(9)=198, a(10)=226,
 *            a(11)=256. For n < 2 the value is undefined; the implementation
 *            returns 0
 * Range:     i8 n <= 6, i16 n <= 47, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

/* Ensure that cache has entries for indices 0..n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)(n + 1);
    if (cache_len > required)
        return;

    seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
    if (!new_cache)
        return; /* allocation failure unlikely for n <= 10000 */

    /* Copy already computed values. */
    if (cache)
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));

    /* Initialize remaining entries to 0. */
    for (size_t i = cache_len; i < required; ++i)
        new_cache[i] = 0;

    /* Base values for indices 0..11. */
    static const seq_t base[12] = {0, 0, -1, 19, 50, 80, 110, 138,
                                   168, 198, 226, 256};
    for (size_t i = 0; i < 12 && i < required; ++i)
        if (i >= cache_len)    /* only set if not already present */
            new_cache[i] = base[i];

    /* Compute recurrence for i >= 12. */
    size_t start_rec = cache_len > 12 ? cache_len : 12;
    for (size_t i = start_rec; i < required; ++i) {
        seq_t val = new_cache[i - 1];     /* a(n-1) */
        val -= new_cache[i - 3];          /* -a(n-3) */
        val += 2 * new_cache[i - 4];      /* +2*a(n-4) */
        val -= new_cache[i - 5];          /* -a(n-5) */
        val += new_cache[i - 7];          /* +a(n-7) */
        val -= new_cache[i - 8];          /* -a(n-8) */
        new_cache[i] = val;
    }

    /* Replace old cache. */
    if (cache)
        free(cache);
    cache = new_cache;
    cache_len = required;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                /* offset is 2; values for n < 2 are irrelevant */
    ensure_cache(n);
    return cache[(size_t)n];
}
